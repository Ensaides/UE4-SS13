// Fill out your copyright notice in the Description page of Project Settings.
#include "SpaceStationGame.h"

#include "OpenCLHelpers.h"
#include "AtmosphericComponent.h"

#include "Kernels/Includes/DebugInfo.h"

#ifdef UE_BUILD_DEBUG
#include "SpaceStationGamePlayerController.h"
#endif

#include "AtmosphericsManager.h"

#define DATA_SIZE 32

AAtmosphericsManager::AAtmosphericsManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//State = AtmosphericsManagerState::OpenCLInitialization;

	//PrimaryActorTick.bCanEverTick = true;
}

/*
void AAtmosphericsManager::Tick(float DeltaSeconds)
{
#ifdef UE_BUILD_DEBUG
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if (Cast<ASpaceStationGamePlayerController>(*it))
		{
			// Update the frame rate
			std::unique_lock<std::mutex> FrameRateLock(FrameRateMutex);
				Cast<ASpaceStationGamePlayerController>(*it)->SetAtmosphericsFPS(FrameRate);
			FrameRateLock.unlock();
		}
	}
#endif

	bComputeAtmospherics = true;
}

void AAtmosphericsManager::Initialize()
{
	if (MaxVoxelCount <= 0) return; // If there aren't any voxels, don't even start the thread

	// Start up the thread
	bThreadRunning = true;
	OpenCLThread = std::thread(&AAtmosphericsManager::GetOpenCLData, this);

	CompleteInitialization();
}

void AAtmosphericsManager::GetOpenCLData()
{
	SetState(AtmosphericsManagerState::OpenCLInitialization);

	while (bThreadRunning)
	{
		// bComputeAtmospherics is the frame limiter, bBenchmarkMode disables it
		if (bComputeAtmospherics || bBenchmarkMode)
		{
			bComputeAtmospherics = false;

			SetState(AtmosphericsManagerState::ComputingAtmospherics);
		}

		SetState(AtmosphericsManagerState::TransferingData);
	}
}

void AAtmosphericsManager::SetState(AtmosphericsManagerState NewState)
{
	State = NewState;

	if (State == AtmosphericsManagerState::OpenCLInitialization)
	{
		OnOpenCLInitialization();
	}
	if (State == AtmosphericsManagerState::ComputingAtmospherics)
	{
		OnComputingAtmospherics();
	}
	if (State == AtmosphericsManagerState::TransferingData)
	{
		OnTransferingData();
	}
}

void AAtmosphericsManager::OnOpenCLInitialization()
{
	// Allocate the voxels on the cpu
	Voxels = new FAtmosVoxel[MaxVoxelCount];

#ifdef UE_BUILD_DEBUG
	// Fill the voxels with a rand set for debug purposes
	srand(FPlatformTime::Seconds());
	for (int i = 0; i < MaxVoxelCount; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			float* Gas = (float*) &Voxels[i].Voxel.Gases;

			Gas[j] = (float) (rand() % 10) + 101.4;
		}

		for (int j = 0; j < 26; j++)
		{
			int NewIndex = rand() % MaxVoxelCount;

			if (NewIndex != i)
			{
				Voxels[i].Voxel .AdjacentVoxels[j] = NewIndex;
			}
			else
			{
				Voxels[i].Voxel.AdjacentVoxels[j] = -1;
			}
		}
	}

	VoxelCount = MaxVoxelCount;
#endif

	cl_int Error = 0;

	// Get platforms
	cl_uint PlatformIDCount = 0;
	clGetPlatformIDs(0, nullptr, &PlatformIDCount);

	Platforms = std::vector<cl_platform_id>(PlatformIDCount);
	clGetPlatformIDs(PlatformIDCount, Platforms.data(), nullptr);

	// Get devices
	cl_uint DeviceIDCount = 0;
	clGetDeviceIDs(Platforms[0], CL_DEVICE_TYPE_GPU, 1, &Device, &DeviceIDCount);

	// Set up the context
	const cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (Platforms[0]),
		0, 0
	};

	Context = clCreateContext(contextProperties, DeviceIDCount, &Device, NULL, NULL, &Error);
	CheckError(Error);
	
	// Create input and output buffers
	Buffer1 = clCreateBuffer(Context, CL_MEM_READ_WRITE, sizeof(FAtmosVoxel) * MaxVoxelCount, NULL, &Error);
	Buffer2 = clCreateBuffer(Context, CL_MEM_READ_WRITE, sizeof(FAtmosVoxel) * MaxVoxelCount, NULL, &Error);
	CheckError(Error);

	// Assign the input/output pointers
	InputBuffer = &Buffer1;
	OutputBuffer = &Buffer2;

#ifdef UE_BUILD_DEBUG
	// Create the buffer for reading debug info
	DebugInfoBuffer = clCreateBuffer(Context, CL_MEM_READ_WRITE, sizeof(OpenCLDebugInfo), NULL, &Error);
	CheckError(Error);
#endif

	// Create a command queue
	CommandQueue = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFILING_ENABLE, &Error);
	CheckError(Error);

	// Write the voxels to the buffers
	CheckError(clEnqueueWriteBuffer(CommandQueue, Buffer1, CL_TRUE, 0, sizeof(FAtmosVoxel) * MaxVoxelCount, Voxels, 0, NULL, NULL));
	CheckError(clEnqueueWriteBuffer(CommandQueue, Buffer2, CL_TRUE, 0, sizeof(FAtmosVoxel) * MaxVoxelCount, Voxels, 0, NULL, NULL));

	// Concatenate the program paths
	FString ComputeAtmosphericsFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir()) + "SpaceStationGame/Kernels/ComputeAtmospherics.cl";
	FString CommitAtmosphericWritesFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir()) + "SpaceStationGame/Kernels/CommitAtmosphericWrites.cl";
	FString GetDebugInfoFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir()) + "SpaceStationGame/Kernels/GetDebugInfo.cl";

	FString IncludeDirectory = "\"" + FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir()) + "SpaceStationGame/Kernels/Includes\"";

	// Compile the programs
	ComputeAtmospherics = OpenCLProgram(ComputeAtmosphericsFilePath, Context, &Device, "ComputeAtmospherics", "-I " + IncludeDirectory);
	CommitAtmosphericWrites = OpenCLProgram(CommitAtmosphericWritesFilePath, Context, &Device, "CommitAtmosphericWrites", "-I " + IncludeDirectory);
#ifdef UE_BUILD_DEBUG
	GetDebugInfo = OpenCLProgram(GetDebugInfoFilePath, Context, &Device, "GetDebugInfo", "-I " + IncludeDirectory);
#endif

	CheckError(clFinish(CommandQueue));

	// Time stamp for calculating FPS
	LastTimeStamp = FPlatformTime::Seconds();
}

void AAtmosphericsManager::OnComputingAtmospherics()
{
	// Swap the input and output buffer pointers
	std::swap(InputBuffer, OutputBuffer);

	std::unique_lock<std::mutex> AtmosLock(AtmosphericsMutex);
		bComputeAtmospherics = false;
		DeltaTime = FPlatformTime::Seconds() - LastTimeStamp;
		LastTimeStamp = FPlatformTime::Seconds();
	AtmosLock.unlock();

#ifdef UE_BUILD_DEBUG
	std::unique_lock<std::mutex> FrameRateLock(FrameRateMutex);
		FrameRate = 1.f / DeltaTime;
	FrameRateLock.unlock();
#endif

	if (DeltaTime > 1.f) DeltaTime = 1.f;

	// Queue Kernel Arguments
	CheckError(clSetKernelArg(ComputeAtmospherics.Kernel, 0, sizeof(cl_mem), (void *)InputBuffer));
	CheckError(clSetKernelArg(ComputeAtmospherics.Kernel, 1, sizeof(cl_mem), (void *)OutputBuffer));
	CheckError(clSetKernelArg(ComputeAtmospherics.Kernel, 2, sizeof(cl_long), (cl_long*)&VoxelCount));
	CheckError(clSetKernelArg(ComputeAtmospherics.Kernel, 3, sizeof(cl_float), (cl_float*)&DeltaTime));

	size_t uintVoxelCount = VoxelCount;

	cl_event newevent;

	CheckError(clEnqueueNDRangeKernel(CommandQueue, ComputeAtmospherics.Kernel, 1, NULL, &uintVoxelCount, NULL, 0, NULL, &newevent));
	CheckError(clWaitForEvents(1, &newevent));

	CheckError(clFinish(CommandQueue));

	
#ifdef UE_BUILD_DEBUG
	CheckError(clSetKernelArg(GetDebugInfo.Kernel, 0, sizeof(cl_mem), (void*)&DebugInfoBuffer));

	size_t uintDebugCount = 1;

	CheckError(clEnqueueNDRangeKernel(CommandQueue, GetDebugInfo.Kernel, 1, NULL, &uintDebugCount, NULL, 0, NULL, NULL));
	CheckError(clFinish(CommandQueue));

	if (bPrintAtmos)
	{
		for (int i = 0; i < 26; i++)
		{
			float GAS;

			CheckError(clEnqueueReadBuffer(CommandQueue, *InputBuffer, CL_TRUE, sizeof(AtmosVoxel) * i + (offsetof(AtmosVoxel, Gases)), sizeof(float), (void*)&GAS, 0, NULL, NULL));

			UE_LOG(SpaceStationGameLog, Warning, TEXT("Voxel %d"), i);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("Input Gas!:\t\t	%f"), GAS);
		}

		for (int i = 0; i < 26; i++)
		{
			float GAS;

			CheckError(clEnqueueReadBuffer(CommandQueue, *OutputBuffer, CL_TRUE, sizeof(AtmosVoxel) * i + (offsetof(AtmosVoxel, Gases)), sizeof(float), (void*)&GAS, 0, NULL, NULL));

			UE_LOG(SpaceStationGameLog, Warning, TEXT("Voxel %d"), i);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("Output Gas!:\t\t	%f"), GAS);
		}

		// Print debug info
		OpenCLDebugInfo DebugInfo;

		CheckError(clEnqueueReadBuffer(CommandQueue, DebugInfoBuffer, CL_TRUE, NULL, sizeof(OpenCLDebugInfo), (void*)&DebugInfo, 0, NULL, NULL));

		UE_LOG(SpaceStationGameLog, Warning, TEXT("DEBUG INFO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
		UE_LOG(SpaceStationGameLog, Warning, TEXT("Size of OpenCL atmos voxel: \t\t %d"), DebugInfo.SizeOfAtmosVoxel);
		UE_LOG(SpaceStationGameLog, Warning, TEXT("Size of UE4 atmos voxel: \t\t %d"), sizeof(AtmosVoxel));
	}
#endif
	
}

void AAtmosphericsManager::OnTransferingData()
{
	if (!bThreadRunning) return;

	// Commit the read operations
	std::unique_lock<std::mutex> BufferReadLock(BufferReadsMutex);
		while (!BufferReads.empty())
		{
			auto NewRead = BufferReads.back();

			std::unique_lock<std::mutex> readlock(*NewRead.DataMutex);
				CheckError(clEnqueueReadBuffer(CommandQueue, *InputBuffer, CL_TRUE, sizeof(FAtmosVoxel) * NewRead.VoxelIndex, sizeof(FAtmosVoxel), (void*)NewRead.VoxelPtr, 0, NULL, NULL));

				if (NewRead.WriteComponent.IsValid())
				{
					NewRead.WriteComponent->ReceiveAtmosUpdate();
				}

				BufferReads.pop_back();
		}
	BufferReadLock.unlock();


	// Commit the write operations
	std::unique_lock<std::mutex> BufferWriteLock(BufferWritesMutex);
		if (!BufferWrites.empty())
		{
			// Call the commit writes kernel
			size_t NumOfElements = BufferWrites.size();

			CheckError(clSetKernelArg(CommitAtmosphericWrites.Kernel, 0, BufferWrites.size() * sizeof(AtmosVoxelWrite) * 5, (void*)&BufferWrites));
			CheckError(clSetKernelArg(CommitAtmosphericWrites.Kernel, 1, sizeof(size_t), (void*)&NumOfElements));
			
			CheckError(clEnqueueNDRangeKernel(CommandQueue, CommitAtmosphericWrites.Kernel, 1, NULL, &NumOfElements, NULL, 0, NULL, NULL));
			CheckError(clFinish(CommandQueue));

			// Empty the voxels once its done
			BufferWrites.clear();
		}
	BufferWriteLock.unlock();
}

void AAtmosphericsManager::CheckError(int Error)
{
	if (Error != CL_SUCCESS)
	{
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OpenCL Error!:\t\t	%s"), *Helpers::get_error_string(Error));
	}
}

void AAtmosphericsManager::CheckBuildError(int Error, cl_program Program, cl_device_id* Device)
{
	if (Error != CL_SUCCESS)
	{
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OpenCL Error!:\t\t	%s"), *Helpers::get_error_string(Error));

		if (Error == CL_BUILD_PROGRAM_FAILURE)
		{
			size_t log_size;
			clGetProgramBuildInfo(Program, *Device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			// Allocate memory for the log
			char* log = new char[log_size];

			// Get the log
			clGetProgramBuildInfo(Program, *Device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

			std::string logstring(log);

			UE_LOG(SpaceStationGameLog, Warning, TEXT("OpenCL Build Error!:\t\t	%s"), *Helpers::ConvertToFString(logstring));

			delete log;
		}
	}
}

void AAtmosphericsManager::BeginDestroy()
{
	Super::BeginDestroy();

	// Close the thread
	bThreadRunning = false;
	if (OpenCLThread.joinable()) OpenCLThread.join();

	// Delete the voxel data
	delete[] Voxels;
}

void AAtmosphericsManager::AddBufferRead(FAtmosVoxel* VoxelPtr, int32 VoxelIndex, std::mutex* DataMutex)
{
	BufferReadStruct NewRead;

	NewRead.VoxelPtr = VoxelPtr;
	NewRead.VoxelIndex = VoxelIndex;
	NewRead.DataMutex = DataMutex;

	std::unique_lock<std::mutex> lock(BufferReadsMutex);
	BufferReads.push_back(NewRead);
};

void AAtmosphericsManager::AddBufferWrite(int32 VoxelIndex, AtmosWriteOperation Operation, cl_float16 Value)
{
	std::unique_lock<std::mutex> lock(BufferWritesMutex);
	// Iterate through the array and see if there is already a write operation queued
	for (int i = 0; i < BufferWrites.size(); i++)
	{
		if (BufferWrites[i][0].Index == VoxelIndex)
		{
			AtmosVoxelWrite NewWrite;

			NewWrite.bValidOperation = true;
			NewWrite.Index = VoxelIndex;
			if (Operation != AtmosWriteOperation::Set)
			{
				NewWrite.Value = OpenCLHelpers::AddFloat16(Value, BufferWrites[i][(uint8)Operation].Value);
			}
			else
			{
				NewWrite.Value = Value;
			}

			BufferWrites[i][(uint8)Operation] = NewWrite;

			return;
		}
	}

	// If there isnt a voxel array already, create a new one
	std::array<AtmosVoxelWrite, 5> NewWrites;

	AtmosVoxelWrite NewWrite;

	NewWrite.bValidOperation = true;
	NewWrite.Index = VoxelIndex;
	NewWrite.Value = Value;

	NewWrites[(uint8)Operation] = NewWrite;

	BufferWrites.push_back(NewWrites);
};

int32 AAtmosphericsManager::AddVoxel()
{
	return -1;
};
*/
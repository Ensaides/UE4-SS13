// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

#include "OpenCL.h"

#include "Tickable.h"
#include "Manager.h"
#include "AtmosVoxel.h"
#include "AtmosphericsManager.generated.h"

struct OpenCLProgram
{
	cl_program Program;
	cl_kernel Kernel;

	OpenCLProgram() {};

	OpenCLProgram(FString FilePath, cl_context context, cl_device_id* device_list, FString kernel_name, FString options);
};

enum AtmosphericsManagerState
{
	OpenCLInitialization,
	ComputingAtmospherics,
	TransferingData
};

struct BufferReadStruct
{
	int32 VoxelIndex;
	// Pointer to the data to write to
	FAtmosVoxel* VoxelPtr;
	// Data mutex, this is used because these operations are on another thread
	std::mutex* DataMutex;

	// For atmospherics components
	TWeakObjectPtr<class UAtmosphericComponent> WriteComponent;
};

UENUM(BlueprintType)
enum class AtmosWriteOperation : uint8
{
	Add,
	Subtract,
	Set,
	SetAdditive,
	SetSubtractive
};

struct BufferWriteStruct
{
	int32 VoxelIndex;
	AtmosWriteOperation Operation;
	cl_float16 Value;
};

/**
*	Manages the atmospherics voxels and uses OpenCL to calculate atmospheric changes
*/
UCLASS()
class SPACESTATIONGAME_API AAtmosphericsManager : public AManager
{
	GENERATED_UCLASS_BODY()

	friend struct OpenCLProgram;

public:
	virtual void Initialize() override;
	virtual void CompleteInitialization() override {};
	virtual void Tick( float DeltaSeconds ) override;
	virtual void BeginDestroy() override;

public:
	// Voxels
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Voxels)
		int32 MaxVoxelCount;

	int32 VoxelCount;

	FAtmosVoxel* Voxels;

private:
	//				OpenCL state
	AtmosphericsManagerState State;

	void SetState(AtmosphericsManagerState NewState);

	// Sets up OpenCL and compiles the kernels
	void OnOpenCLInitialization();
	
	// Computes atmospherics and performs read operations on the OpenCL voxel buffer at the same time
	void OnComputingAtmospherics();

	// Transfers data from the output to the input, performs write operations on the input buffer
	void OnTransferingData();

	//				Threading members
	std::atomic<bool> bThreadRunning;
	std::atomic<bool> bComputingAtmospherics;

	//				OpenCL members
	std::thread OpenCLThread;

	std::vector<cl_platform_id> Platforms;
	cl_device_id Device;

	// These are the input/output buffers, they are swapped after atmospherics are computed and assigned to the input/output pointers
	cl_mem Buffer1;
	cl_mem Buffer2;

	cl_mem* InputBuffer;
	cl_mem* OutputBuffer;

	cl_event event;

	cl_command_queue CommandQueue;

	cl_context Context;

	OpenCLProgram ComputeAtmospherics;

	// OpenCL error checking
	static void CheckError(int Error);

	static void CheckBuildError(int Error, cl_program Program, cl_device_id* Device);

	// Thread function
	void GetOpenCLData();

private:
	std::atomic<bool> bComputeAtmospherics;

	// Last time stamp (in seconds) is used to calculate delta time
	double LastTimeStamp;
	float DeltaTime;
	std::mutex AtmosphericsMutex;

public:
	// Disables the frame limiter, unties the computations from the frame rate
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Computations)
		bool bBenchmarkMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Atmos)
		bool bPrintAtmos;

#ifdef UE_BUILD_DEBUG
private:
	float FrameRate;
	std::mutex FrameRateMutex;
#endif

private:
	std::vector<FAtmosVoxel> NewVoxels;
	std::mutex NewVoxelsMutex;

	std::vector<BufferReadStruct> BufferReads;
	std::mutex BufferReadsMutex;

	std::vector<BufferWriteStruct> BufferWrites;
	std::mutex BufferWritesMutex;

public:
	// TO DO: Add a blueprint version of this somehow
	void AddBufferRead(FAtmosVoxel* VoxelPtr, int32 VoxelIndex, std::mutex* DataMutex)
	{
		BufferReadStruct NewRead;

		NewRead.VoxelPtr = VoxelPtr;
		NewRead.VoxelIndex = VoxelIndex;
		NewRead.DataMutex = DataMutex;

		std::unique_lock<std::mutex> lock(BufferReadsMutex);
			BufferReads.push_back(NewRead);
	};

	void AddBufferWrite(int32 VoxelIndex, AtmosWriteOperation Operation, cl_float16 Value)
	{
		BufferWriteStruct NewWrite;

		NewWrite.VoxelIndex = VoxelIndex;
		NewWrite.Operation = Operation;
		NewWrite.Value = Value;

		std::unique_lock<std::mutex> lock(BufferWritesMutex);
			BufferWrites.push_back(NewWrite);
	};

	int32 AddVoxel()
	{
		
	};
};
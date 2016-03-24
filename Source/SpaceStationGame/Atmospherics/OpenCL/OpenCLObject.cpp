// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"

#include <fstream>
#include <iostream>
#include "StringHelpers.h"


#include "OpenCLObject.h"

#define MAX_SOURCE_SIZE (0x100000)

/* Error Codes */

#define CL_SUCCESS 0
#define CL_DEVICE_NOT_FOUND -1
#define CL_DEVICE_NOT_AVAILABLE -2
#define CL_COMPILER_NOT_AVAILABLE -3
#define CL_MEM_OBJECT_ALLOCATION_FAILURE -4
#define CL_OUT_OF_RESOURCES -5
#define CL_OUT_OF_HOST_MEMORY -6
#define CL_PROFILING_INFO_NOT_AVAILABLE -7
#define CL_MEM_COPY_OVERLAP -8
#define CL_IMAGE_FORMAT_MISMATCH -9
#define CL_IMAGE_FORMAT_NOT_SUPPORTED -10
#define CL_BUILD_PROGRAM_FAILURE -11
#define CL_MAP_FAILURE -12

#define CL_INVALID_VALUE -30
#define CL_INVALID_DEVICE_TYPE -31
#define CL_INVALID_PLATFORM -32
#define CL_INVALID_DEVICE -33
#define CL_INVALID_CONTEXT -34
#define CL_INVALID_QUEUE_PROPERTIES -35
#define CL_INVALID_COMMAND_QUEUE -36
#define CL_INVALID_HOST_PTR -37
#define CL_INVALID_MEM_OBJECT -38
#define CL_INVALID_IMAGE_FORMAT_DESCRIPTOR -39
#define CL_INVALID_IMAGE_SIZE -40
#define CL_INVALID_SAMPLER -41
#define CL_INVALID_BINARY -42
#define CL_INVALID_BUILD_OPTIONS -43
#define CL_INVALID_PROGRAM -44
#define CL_INVALID_PROGRAM_EXECUTABLE -45
#define CL_INVALID_KERNEL_NAME -46
#define CL_INVALID_KERNEL_DEFINITION -47
#define CL_INVALID_KERNEL -48
#define CL_INVALID_ARG_INDEX -49
#define CL_INVALID_ARG_VALUE -50
#define CL_INVALID_ARG_SIZE -51
#define CL_INVALID_KERNEL_ARGS -52
#define CL_INVALID_WORK_DIMENSION -53
#define CL_INVALID_WORK_GROUP_SIZE -54
#define CL_INVALID_WORK_ITEM_SIZE -55
#define CL_INVALID_GLOBAL_OFFSET -56
#define CL_INVALID_EVENT_WAIT_LIST -57
#define CL_INVALID_EVENT -58
#define CL_INVALID_OPERATION -59
#define CL_INVALID_GL_OBJECT -60
#define CL_INVALID_BUFFER_SIZE -61
#define CL_INVALID_MIP_LEVEL -62
#define CL_INVALID_GLOBAL_WORK_SIZE -63

const FString get_error_string(cl_int err) {
	switch (err) {
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";

	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	default: return "Unknown OpenCL error";
	}
}

UOpenCLObject::UOpenCLObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//bWantsBeginPlay = true;
}

void UOpenCLObject::Initialize()
{
	bThreadRunning = true;

	OpenCLThread = std::thread(&UOpenCLObject::SetUpOpenCL, this);

	//SetUpOpenCL();
}

void UOpenCLObject::GetOpenCLData()
{
	SetUpOpenCL();
}

void UOpenCLObject::SetUpOpenCL()
{
	// Please kill me
	// I fucking hate opencl
	cl_int Error;

	// Get platforms
	cl_uint PlatformIDCount = 0;
	clGetPlatformIDs(0, nullptr, &PlatformIDCount);

	Platforms = std::vector<cl_platform_id>(PlatformIDCount);
	clGetPlatformIDs(PlatformIDCount, Platforms.data(), nullptr);

	// Get devices
	cl_uint DeviceIDCount = 0;
	clGetDeviceIDs(Platforms[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &DeviceIDCount);

	const cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM,
		reinterpret_cast<cl_context_properties> (Platforms[0]),
		0, 0
	};

	Context = clCreateContext(contextProperties, DeviceIDCount, Devices.data(), nullptr, nullptr, &Error);

	std::FILE* programHandle;
	size_t programSize;
	char* programBuffer;

	// get size of kernel source
	fopen_s(&programHandle, "C:/Unreal Projects/SpaceStationGame/Source/SpaceStationGame/Atmospherics/OpenCL/Kernel/AtmosKernels.cl", "r");
	fseek(programHandle, 0, SEEK_END);
	programSize = ftell(programHandle);
	rewind(programHandle);

	// read kernel source into buffer
	programBuffer = (char*)malloc(programSize + 1);
	programBuffer[programSize] = '\0';
	fread(programBuffer, sizeof(char), programSize, programHandle);
	fclose(programHandle);

	Program = clCreateProgramWithSource(Context, 1, (const char**)&programBuffer, &programSize, &Error);
	
	free(programBuffer);

	Error = clBuildProgram(Program, 0, NULL, NULL, NULL, NULL);

	if (Error != CL_SUCCESS)
	{
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OpenCL Error!:\t\t	%s"), *get_error_string(Error))
	}

	Kernel = clCreateKernel(Program, "hello", &Error);

	cl_command_queue CommandQueue = clCreateCommandQueue(Context, Devices[0], 0, &Error);

	Error = clEnqueueTask(CommandQueue, Kernel, 0, NULL, NULL);
}

void UOpenCLObject::Tick(float DeltaTime)
{
	if (!bComputingAtmospherics)
	{

	}
}

void UOpenCLObject::BeginDestroy()
{
	Super::BeginDestroy();

	bThreadRunning = false;

	if (OpenCLThread.joinable()) OpenCLThread.join();
}

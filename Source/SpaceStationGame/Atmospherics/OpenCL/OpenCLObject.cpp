// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"

#include <fstream>

#include "OpenCLObject.h"

#define MAX_SOURCE_SIZE (0x100000)

UOpenCLObject::UOpenCLObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//bWantsBeginPlay = true;
}

void UOpenCLObject::Initialize()
{
	bThreadRunning = true;

	OpenCLThread = std::thread(&UOpenCLObject::GetOpenCLData, this);
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

	std::ifstream file("/Kernel/AtmosKernels.cl");
	std::string prog(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
	file.close();

	size_t progLength = prog.length() + 1;

	Program = clCreateProgramWithSource(Context, 1, (const char**)&prog, &progLength, &Error);
	Error = clBuildProgram(Program, 0, NULL, NULL, NULL, NULL);
}

void UOpenCLObject::Tick(float DeltaTime)
{
	if (!bComputingAtmospherics)
	{

	}
}

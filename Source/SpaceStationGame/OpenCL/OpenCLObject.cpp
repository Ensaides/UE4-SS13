// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "OpenCLObject.h"

void UOpenCLObject::SetUpOpenCL()
{
	try 
	{
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

		Context = clCreateContext(contextProperties, deviceIdCount, deviceIds.data(), nullptr, nullptr, &error);
	}
}



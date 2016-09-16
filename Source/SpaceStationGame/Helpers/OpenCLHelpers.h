// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceStationGame.h"
#include "OpenCL.h"

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

namespace Helpers
{
	FString get_error_string(cl_int err) 
	{
		switch (err) 
		{
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
}

class OpenCLHelpers
{
public:
	static cl_float16 AddFloat16(cl_float16 Lhs, cl_float16 Rhs)
	{
		cl_float16 ReturnValue;

		auto LhsPtr = (float*)&Lhs;
		auto RhsPtr = (float*)&Rhs;
		auto ReturnValuePtr = (float*)&ReturnValue;

		for (int i = 0; i < 16; i++)
			ReturnValuePtr[i] = LhsPtr[i] + RhsPtr[i];

		return ReturnValue;
	}
};
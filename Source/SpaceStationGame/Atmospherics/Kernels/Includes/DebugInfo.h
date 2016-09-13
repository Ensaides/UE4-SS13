#pragma once

#ifdef __cplusplus
#include "OpenCL.h"
#endif

#ifdef __cplusplus
typedef struct OpenCLDebugInfo
{
	cl_long SizeOfAtmosVoxel;
};
#else
typedef struct
{
	long SizeOfAtmosVoxel;
} DebugInfo;
#endif

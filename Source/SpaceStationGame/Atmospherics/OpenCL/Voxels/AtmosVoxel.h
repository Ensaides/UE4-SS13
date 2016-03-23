#pragma once

#include "CL/cl.h"

// Put these members in larger to smaller order for memory alignment
typedef struct 
{
	cl_float16 Gasses;
	int AdjacentVoxels[26];
	bool bNeedsUpdate;
} AtmosVoxel;
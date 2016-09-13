#include "AtmosVoxel.h"
#include "DebugInfo.h"

__kernel void GetDebugInfo(__global DebugInfo* OutputBuffer)
{
	OutputBuffer->SizeOfAtmosVoxel = sizeof(AtmosVoxel);
}
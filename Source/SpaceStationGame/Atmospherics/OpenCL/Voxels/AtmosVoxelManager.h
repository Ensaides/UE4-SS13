#pragma once

#include "Object.h"
#include <vector>

#include "AtmosVoxel.h"

class UAtmosVoxelManager
{
protected:
	static std::vector<AtmosVoxel> AtmosVoxels;

public:
	static void AddVoxel(AtmosVoxel) { AtmosVoxels.Push(AtmosVoxel); };

	static void RemoveVoxel(int Index) { AtmosVoxels.Remove(Index); };

	static std::vector<AtmosVoxel>& GetVoxels() { return AtmosVoxels; };
};
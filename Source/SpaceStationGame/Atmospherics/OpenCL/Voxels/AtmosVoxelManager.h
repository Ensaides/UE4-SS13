#pragma once

#include "Object.h"
#include <vector>

#include "AtmosVoxelManager.generated.h"

class AtmosVoxel;

UCLASS()
class SPACESTATIONGAME_API UAtmosVoxelManager : public UObject
{
	GENERATED_BODY()

private:
	static std::vector<AtmosVoxel> AtmosVoxels;

public:
	static void AddVoxel(AtmosVoxel) { AtmosVoxels.Push(AtmosVoxel); };

	static void RemoveVoxel(int Index) { AtmosVoxels.Remove(Index); };

	static std::vector<AtmosVoxel>& GetVoxels() { return AtmosVoxels; };
};
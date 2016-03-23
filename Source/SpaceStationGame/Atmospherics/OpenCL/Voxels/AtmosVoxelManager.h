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
	std::vector<AtmosVoxel> AtmosVoxels;

public:
	void AddVoxel(AtmosVoxel) { AtmosVoxels.Push(AtmosVoxel); };

	void RemoveVoxel(int Index) { AtmosVoxels.Remove(Index); };
};
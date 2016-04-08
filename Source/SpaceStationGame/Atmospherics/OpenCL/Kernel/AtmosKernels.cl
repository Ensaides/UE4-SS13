typedef struct
{
	// Gasses
	float16 Gases;

	// Pos
	float XPos;
	float YPos;
	float ZPos;
	
	// ID
	int AdjacentVoxels[8];
	int ValidVoxelIndex;
	
	// State
	bool bValidVoxel;
} AtmosVoxel;

#define GAS_UPDATE_DIFFERENCE 5.f
#define GAS_UPDATE_INTERP_TIME 4.f

__kernel void ComputeAtmospherics(__global AtmosVoxel* InputAtmosVoxels, __global AtmosVoxel* OutputAtmosVoxels, __global int* UpdateAtmosVoxels, int UpdateAtmosVoxelsCount, float DeltaSeconds)
{
	int i = get_global_id(0);

	if(i < UpdateAtmosVoxelsCount)
	{
		AtmosVoxel WorkerVoxel = InputAtmosVoxels[UpdateAtmosVoxels[i]];

		AtmosVoxel AdjacentVoxels[8];

		int intValidAdjacentVoxels;
		bool ValidAdjacentVoxels[8];

		float InterpMul = (DeltaSeconds / GAS_UPDATE_INTERP_TIME);

		InterpMul *= InterpMul; 

		if (InterpMul > 1.f) InterpMul = 1.f;

		for (int i = 0; i < 8; i++)
		{
			if (WorkerVoxel.AdjacentVoxels[i] > -1)
			{
				AdjacentVoxels[i] = InputAtmosVoxels[WorkerVoxel.AdjacentVoxels[i]];

				intValidAdjacentVoxels++;

				ValidAdjacentVoxels[i] = true;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			if (ValidAdjacentVoxels[i])
			{
				bool bUpdateVoxel = false;

				float16 DeltaGases = WorkerVoxel.Gases - AdjacentVoxels[i].Gases;

				float* DeltaGasesArray = (float*) &DeltaGases;

				for (int i = 0; i < 16; i++)
				{
					if (DeltaGasesArray[i] > GAS_UPDATE_DIFFERENCE || DeltaGasesArray[i] < -GAS_UPDATE_DIFFERENCE)
					{
						bUpdateVoxel = true;

						break;
					}
				}

				if (bUpdateVoxel)
				{
					float16 GasAverage = DeltaGases / 2.f;

					GasAverage *= InterpMul;

					GasAverage /= intValidAdjacentVoxels;

					WorkerVoxel.Gases += GasAverage;

					//AdjacentVoxels[i].Gases -= GasAverage;
				}
			}
		}

		OutputAtmosVoxels[UpdateAtmosVoxels[i]] = WorkerVoxel;
	}
}



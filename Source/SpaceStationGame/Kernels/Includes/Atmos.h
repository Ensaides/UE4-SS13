// Defines the atmos globals

typedef struct
{
	// Gasses
	float16 Gases;

	// ID
	int AdjacentVoxels[26];
	
	// State
	bool bValidVoxel;
} AtmosVoxel;

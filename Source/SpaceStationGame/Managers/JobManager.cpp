#include "SpaceStationGame.h"
#include "JobManager.h"

AJobManager::AJobManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InitializeJobs();
	BP_InitializeJobs();
}

void AJobManager::InitializeJobs()
{
	// Nothing here yet, will add jobs when they are created
}
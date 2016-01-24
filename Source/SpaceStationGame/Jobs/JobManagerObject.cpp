#include "SpaceStationGame.h"
#include "JobManagerObject.h"

void UJobManagerObject::AddJob(TSubclassOf<UJobObject> NewJob)
{
	JobArray.Add(NewJob);
}
#include "SpaceStationGame.h"
#include "JobManagerObject.h"

void UJobManagerObject::AddJob(TSubclassOf<UJobObject> NewJob, FString JobName)
{
	JobArray.Add(NewJob);

	JobArrayNames.Add(JobName);
}
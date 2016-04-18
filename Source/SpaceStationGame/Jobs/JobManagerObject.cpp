#include "SpaceStationGame.h"
#include "JobManagerObject.h"

void UJobManagerObject::Initialize()
{
	// Please fix this, the object shouldn't be staying around after the game ends
	JobArrayNames = {};
	JobArray = {};

	SetUpJobs();
}

void UJobManagerObject::AddJob(TSubclassOf<UJobObject> NewJob, FString JobName)
{
	JobArray.Add(NewJob);

	JobArrayNames.Add(JobName);
}

uint8 UJobManagerObject::GetJobIndex(TSubclassOf<UJobObject> Job)
{
	return JobManagerObject->JobArray.Find(Job);
}
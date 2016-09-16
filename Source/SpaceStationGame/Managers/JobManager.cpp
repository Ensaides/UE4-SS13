#include "SpaceStationGame.h"
#include "Human.h"
#include "SpaceStationGamePlayerController.h"
#include "JobManager.h"

TArray<FJobClass> AJobManager::JobClasses;

AJobManager::AJobManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InitializeJobs();
}

void AJobManager::InitializeJobs_Implementation()
{
	// Nothing here yet, will add jobs when they are created
}

void AJobManager::SetupJob(APawn* PlayerPawn)
{
	// Find the job that the player wants and spawn the job object
	auto HumanPawn = Cast<AHuman>(PlayerPawn);
	if (HumanPawn)
	{
		auto Controller = Cast<ASpaceStationGamePlayerController>(HumanPawn->GetController());
		auto World = GetWorld();

		if (World && Controller)
		{
			auto Job = Controller->StartingJob;
			auto JobObject = NewObject<UJob>(HumanPawn, Job);

			// Set up the inventory and send the player the job message
			JobObject->ConstructInventory(HumanPawn);
			Controller->Client_AddChatMessage(JobObject->JobMessage, MESSAGE_NOTIFICATION_COLOR);
		}
	}
}
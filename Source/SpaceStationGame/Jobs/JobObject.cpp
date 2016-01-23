// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "IDCard.h"
#include "Jobs.h"
#include "JobObject.h"

UJobObject::UJobObject()
{
	UWorld* const World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;

		AIDCard* IDCard = World->SpawnActor<AIDCard>(AIDCard::StaticClass(), SpawnParams);

		//Reimplement this please
		/*if (Cast<ASpaceStationGameCharacter>(Owner))
		{
			IDCard->ServerSetAssignedName(Cast<ASpaceStationGameCharacter>(Owner)->GetPawnName());

			IDCard->ServerSetJob(EJobs::EJob_Captain);
		}*/

		StartingInventory.Items.Add(IDCard);
		StartingInventory.Index.Add((uint8)EHumanInventorySlot::EHumanInven_Slot_ID);
	}
}

UJobObject* UJobObject::GetJobObjectFromJob(EJobs Job)
{
	if (Job == EJobs::EJob_Captain)
	{
		return NewObject<UJobObject>();
	}
	else
	{
		return NewObject<UJobObject>();
	}
}



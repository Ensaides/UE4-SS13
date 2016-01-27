// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGameCharacter.h"
#include "SpaceStationGamePlayerController.h"
#include "AntagonistRoles.h"
#include "RoundTypeTraitor.h"

ARoundTypeTraitor::ARoundTypeTraitor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// One in every 8 people is a traitor
	TraitorRatio = 8;

	NumOfObjectives = 2;
}

void ARoundTypeTraitor::InitObjectives()
{
	TraitorObjectives[0] = &ARoundTypeTraitor::Objective_Assassinate;
}

void ARoundTypeTraitor::InitializeRound_Implementation()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceStationGameCharacter::StaticClass(), FoundActors);

	// If no one is playing the game, return
	if (FoundActors.Num() == 0) return;

	TArray<AActor*> FoundActorsCopy = FoundActors;

	int32 NumOfTraitors = FoundActors.Num() / TraitorRatio;
	if (NumOfTraitors == 0) NumOfTraitors = 1;

	// For each traitor number, find a person to become a traitor
	for (int32 i = 0; i < NumOfTraitors;)
	{
		// If no one wants to be a traitor and the array is cleared out
		if (FoundActorsCopy.Num() == 0)
		{
			//Pick a traitor anyway using the original array
			int32 RandomNumber = FMath::RandRange(0, FoundActors.Num() - 1);

			FoundActors.RemoveSingle(FoundActorsCopy[RandomNumber]);
			FoundActorsCopy.RemoveAt(RandomNumber);

			i++;
		}
		else
		{
			int32 RandomNumber = FMath::RandRange(0, FoundActorsCopy.Num() - 1);

			auto FoundCharacter = Cast<ASpaceStationGameCharacter>(FoundActorsCopy[RandomNumber]);

			auto FoundCharacterController = Cast<ASpaceStationGamePlayerController>(FoundCharacter->GetController());

			// If this character doesn't want to be a traitor
			if (FoundCharacterController->PreferredAntagonistRole.bTraitor == false)
			{
				// Remove them from the array and redo the iterator
				FoundActorsCopy.RemoveAt(RandomNumber);

			}
			else
			{
				// If the person wants to be a traitor, give them the role
				FoundCharacter->SetAntagonistRole(EAntagonistRoles::EAntagonistRole_Traitor);

				// Remove the character from both arrays so that he can't be found again
				FoundActors.RemoveSingle(FoundActorsCopy[RandomNumber]);
				FoundActorsCopy.RemoveAt(RandomNumber);

				i++;
			}
		}
	}
}

void ARoundTypeTraitor::AddObjective(int32 ObjectiveIndex, ASpaceStationGameCharacter* Character)
{
	//valid range check
	if (ObjectiveIndex >= TRAITOR_OBJECTIVES || ObjectiveIndex < 0) return;
	//~~

	//Special Thanks to Epic for this Syntax
	(this->* (TraitorObjectives[ObjectiveIndex]))(Character);

	//the above line plays the appropriate function based on the passed in index!
}

void ARoundTypeTraitor::SetUpTraitorCharacter(ASpaceStationGameCharacter* Character)
{
	for (int32 i = 0; i < NumOfObjectives; i++)
	{
		int32 RandomTraitorObjective = FMath::RandRange(0, TRAITOR_OBJECTIVES - 1);

		AddObjective(RandomTraitorObjective, Character);
	}
}

void ARoundTypeTraitor::Objective_Assassinate(ASpaceStationGameCharacter* Character)
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceStationGameCharacter::StaticClass(), FoundActors);

	// Don't give the character the objective to kill himself
	FoundActors.RemoveSingle(Character);

	// If hes the only traitor, he can't kill anyone
	if (FoundActors.Num() == 1) return;

	int32 CharacterIndex = FMath::RandRange(0, FoundActors.Num() - 1);

	TArray<FString> Objective = Character->Notes;

	ASpaceStationGameCharacter* FoundCharacter = Cast<ASpaceStationGameCharacter>(FoundActors[CharacterIndex]);

	Objective.Add("Assassinate " + FoundCharacter->GetPawnName());

	Character->Server_SetNotes(Objective);


}
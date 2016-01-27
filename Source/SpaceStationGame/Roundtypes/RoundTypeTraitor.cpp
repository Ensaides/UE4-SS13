// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGameCharacter.h"
#include "AntagonistRoles.h"
#include "RoundTypeTraitor.h"

ARoundTypeTraitor::ARoundTypeTraitor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// One in every 8 people is a traitor
	TraitorRatio = 8;
}

void ARoundTypeTraitor::InitializeRound_Implementation()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceStationGameCharacter::StaticClass(), FoundActors);

	int32 NumOfTraitors = FoundActors.Num() / TraitorRatio;
	if (NumOfTraitors == 0) NumOfTraitors = 1;

	// for each traitor number, find a person to become a traitor
	for (int32 i = 0; i <= NumOfTraitors;)
	{
		int32 RandomNumber = FMath::RandRange(0, FoundActors.Num());
		
		Cast<ASpaceStationGameCharacter>(FoundActors[RandomNumber])->SetAntagonistRole(EAntagonistRoles::EAntagonistRole_Traitor);

		i++;
	}
}



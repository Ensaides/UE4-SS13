// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "Human.h"
#include "Traitor.h"

ATraitor::ATraitor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TraitorRatio = 8;
	MaxTraitors = 4;
	MinTraitors = 1;
}

void ATraitor::StartRound()
{
	Super::StartRound();

	auto ReadyPlayers = GetReadyPlayers();

	if (ReadyPlayers.Num() > 0)
	{
		// Get the num of traitors
		auto NumTraitors = ReadyPlayers.Num() / TraitorRatio;

		// Clamp it
		if (NumTraitors < MinTraitors)
			NumTraitors = MinTraitors;

		if (NumTraitors > MaxTraitors)
			NumTraitors = MaxTraitors;

		// Randomly select the traitors
		for (int i = 0; i < NumTraitors; i++)
		{
			auto NewTraitorIndex = FGenericPlatformMath::Rand() % ReadyPlayers.Num();

			ReadyPlayers[NewTraitorIndex]->AddChatMessage("You are now a traitor!", MESSAGE_COLOR_RED);
			SetupPlayerObjectives(ReadyPlayers[NewTraitorIndex]);

			ReadyPlayers.RemoveAt(NewTraitorIndex);
		}
	}
}

void ATraitor::SetupPlayerObjectives(ASpaceStationGamePlayerController* Player)
{
	// Add the primary objective
	if (GetPrimaryObjectives().Num() > 0)
	{
		auto NewObjectiveIndex = FGenericPlatformMath::Rand() % GetPrimaryObjectives().Num();
		auto NewObjective = NewObject<UObjective>(Player, GetPrimaryObjectives()[NewObjectiveIndex]);

		Player->Objectives.Add(NewObjective);
		Player->AddChatMessage("Primary objective:", MESSAGE_COLOR_RED);
		Player->AddChatMessage(NewObjective->GetObjectiveMessage(), MESSAGE_COLOR_RED);
	}

	// Add the secondary objective
	if (GetSecondaryObjectives().Num() > 0)
	{
		auto NewObjectiveIndex = FGenericPlatformMath::Rand() % GetSecondaryObjectives().Num();
		auto NewObjective = NewObject<UObjective>(Player, GetSecondaryObjectives()[NewObjectiveIndex]);

		Player->Objectives.Add(NewObjective);
		Player->AddChatMessage("Secondary objective:", MESSAGE_COLOR_RED);
		Player->AddChatMessage(NewObjective->GetObjectiveMessage(), MESSAGE_COLOR_RED);
	}
}

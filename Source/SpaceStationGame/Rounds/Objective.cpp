// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SpaceStationGame.h"
#include "Objective.h"

UObjective::UObjective(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UObjective::InitializeObjective_Implementation()
{

}

FString UObjective::GetObjectiveMessage_Implementation()
{
	return FString("");
}
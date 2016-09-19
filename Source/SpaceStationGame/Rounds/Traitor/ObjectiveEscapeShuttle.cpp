// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SpaceStationGame.h"
#include "Traitor.h"
#include "ObjectiveEscapeShuttle.h"

UObjectiveEscapeShuttle::UObjectiveEscapeShuttle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FString UObjectiveEscapeShuttle::GetObjectiveMessage_Implementation()
{
	return FString("Escape on the shuttle alive.");
}

TRAITOR_ADD_SECONDARY_OBJECTIVE NewObjective(UObjectiveEscapeShuttle::StaticClass());
// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SpaceStationGame.h"
#include "Traitor.h"
#include "ObjectiveKill.h"

UObjectiveKill::UObjectiveKill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FString UObjectiveKill::GetObjectiveMessage_Implementation()
{
	return FString("Go kill some dude.");
}

TRAITOR_ADD_PRIMARY_OBJECTIVE NewObjective(UObjectiveKill::StaticClass());
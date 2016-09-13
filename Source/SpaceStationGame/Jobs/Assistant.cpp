// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "JobManager.h"
#include "Assistant.h"

UAssistant::UAssistant(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultAccess = FAccess();
	JobName = "Assistant";
	JobMessage = "";
}

void UAssistant::ConstructInventory_Implementation(AHuman* OwningHuman)
{

}

DECLARE_JOB AssistantJob("Assistant", UAssistant::StaticClass());
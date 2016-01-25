// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "IDCard.h"
#include "JobObject.h"

UJobObject::UJobObject()
{
	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		SetUpJobObject();
	}
}

void UJobObject::Add(AItem* Item, uint8 inIndex)
{
	StartingInventory.Items.Add(Item);
	StartingInventory.Index.Add(inIndex);
}

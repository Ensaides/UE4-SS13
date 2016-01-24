// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "IDCard.h"
#include "JobObject.h"

UJobObject::UJobObject()
{
	SetUpJobObject();
}

void UJobObject::Add(AItem* Item, uint8 inIndex)
{
	StartingInventory.Items.Add(Item);
	StartingInventory.Index.Add(inIndex);
}

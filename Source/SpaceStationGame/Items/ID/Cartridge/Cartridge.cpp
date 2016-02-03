// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "Cartridge.h"

ACartridge::ACartridge(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bReplicateMovement = true;

	Mesh->SetIsReplicated(true);

	PrimaryActorTick.bCanEverTick = true;
}
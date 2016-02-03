// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "PDA.h"

APDA::APDA(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bReplicateMovement = true;

	Mesh->SetIsReplicated(true);

	PrimaryActorTick.bCanEverTick = true;
}
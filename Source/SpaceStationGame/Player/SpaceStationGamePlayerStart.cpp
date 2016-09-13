// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "SpaceStationGamePlayerStart.h"

ASpaceStationGamePlayerStart::ASpaceStationGamePlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CapsuleComponent->SetCapsuleSize(55, 122);
}



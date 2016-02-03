// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "UnrealNetwork.h"
#include "PDA.h"

APDA::APDA(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bReplicateMovement = true;

	Mesh->SetIsReplicated(true);

	PrimaryActorTick.bCanEverTick = true;
}

void APDA::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APDA, AssignedJob, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APDA, InsertedID, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APDA, InsertedCartridge, COND_OwnerOnly);
}
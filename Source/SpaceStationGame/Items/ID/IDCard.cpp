// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "UnrealNetwork.h"
#include "IDCard.h"

AIDCard::AIDCard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AssignedName = TEXT("Dicks");

	bReplicates = true;
	bReplicateMovement = true;

	Mesh->SetIsReplicated(true);

	PrimaryActorTick.bCanEverTick = true;
}

void AIDCard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AIDCard, AssignedName, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AIDCard, AssignedJob, COND_OwnerOnly);
}

void AIDCard::ServerSetAssignedName_Implementation(const FString& NewName)
{
	AssignedName = NewName;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, AssignedName);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, NewName);
}

FString AIDCard::GetAssignedName()
{
	return AssignedName;
}

//void AIDCard::SetAssignedName_Implementation(FString NewName)
//{
//
//}

void AIDCard::ServerSetJob_Implementation(uint8 Job)
{
	AssignedJob = Job;
}
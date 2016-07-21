// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "InstancedMeshContainer.h"


// Sets default values
AInstancedMeshContainer::AInstancedMeshContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancedMesh = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("Instanced Mesh"));
	RootComponent = InstancedMesh;

	InstancedMesh->SetIsReplicated(false);

	InstancedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//This actor should ONLY run on the client side, it is a model representation and nothing more
	bReplicates = false;
	bReplicateMovement = false;

}

void AInstancedMeshContainer::SetMesh(UStaticMesh* NewMesh)
{
	InstancedMesh->SetStaticMesh(NewMesh);
}

int32 AInstancedMeshContainer::AddInstance(const FTransform& WorldTransform)
{
	return InstancedMesh->AddInstanceWorldSpace(WorldTransform);
}

void AInstancedMeshContainer::RemoveInstance(int32 Index)
{
	InstancedMesh->RemoveInstance(Index);
}
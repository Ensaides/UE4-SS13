// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "InstanceManager.h"

#include "InstancedItem.h"
#include "InstancedMeshContainer.h"

AInstanceManager::AInstanceManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool AInstanceManager::CreateNewInstanceContainer(AInstancedItem* InstanceActor)
{
	// If we don't find an instance container, create a new one
	if (InstanceContainers.find(InstanceActor->StaticClass()) == InstanceContainers.end())
	{
		UWorld* const World = GetWorld();

		FActorSpawnParameters NewParams;

		NewParams.Owner = this;
		NewParams.bNoFail = true;

		int32 InstIndex = InstancedMeshContainers.Add(World->SpawnActor<AInstancedMeshContainer>(AInstancedMeshContainer::StaticClass(), NewParams));

		InstancedMeshContainers[InstIndex]->SetMesh(InstanceActor->Mesh->StaticMesh);

		InstanceContainers[InstanceActor->StaticClass()] = InstancedMeshContainers[InstIndex];

		return true;
	}

	return false;
}

void AInstanceManager::CreateNewInstance(AInstancedItem* InstanceActor)
{
	CreateNewInstanceContainer(InstanceActor);

	InstanceActor->InstanceIndex = InstanceContainers[InstanceActor->StaticClass()]->AddInstance(InstanceActor->GetTransform());
}

void AInstanceManager::RemoveInstance(AInstancedItem* InstanceActor)
{
	InstanceContainers[InstanceActor->StaticClass()]->RemoveInstance(InstanceActor->GetInstanceIndex());
}

void AInstanceManager::RemoveInstance(TSubclassOf<AInstancedItem> InstanceItemClass, int32 Index)
{
	InstanceContainers[InstanceItemClass]->RemoveInstance(Index);
}
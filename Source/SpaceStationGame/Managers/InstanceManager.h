// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Managers/Manager.h"

#include <map>

#include "InstanceManager.generated.h"

class AInstancedItem;
class AInstancedMeshContainer;

// Manages static mesh instances, used for things like floors and walls and the like
UCLASS()
class SPACESTATIONGAME_API AInstanceManager : public AManager
{
	GENERATED_UCLASS_BODY()
		
	// Checks if this actor already has an instance container, will create one if not. Returns true if created a new container
	bool CreateNewInstanceContainer(AInstancedItem* InstanceActor);

	void CreateNewInstance(AInstancedItem* InstanceActor);

	void RemoveInstance(AInstancedItem* InstanceActor);

	void RemoveInstance(TSubclassOf<AInstancedItem> InstanceItemClass, int32 Index);

public:
	// This is only used to avoid the actors getting garbage collected
	UPROPERTY()
		TArray<AInstancedMeshContainer*> InstancedMeshContainers;

private:
	std::map<TSubclassOf<AInstancedItem>, AInstancedMeshContainer*> InstanceContainers;
};

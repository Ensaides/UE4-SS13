// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InstancedMeshContainer.generated.h"

// Wrapper actor for an instanced static mesh component
UCLASS()
class SPACESTATIONGAME_API AInstancedMeshContainer : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Instanced mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		class UInstancedStaticMeshComponent* InstancedMesh;

	void SetMesh(class UStaticMesh* NewMesh);

	int32 AddInstance(const FTransform& WorldTransform);

	void RemoveInstance(int32 Index);
};

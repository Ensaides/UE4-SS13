// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "InstancedItem.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API AInstancedItem : public AItem
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance)
		int32 InstanceIndex;

	int32 GetInstanceIndex() const { return InstanceIndex; };
	

public:
	virtual void BeginPlay() override;
};

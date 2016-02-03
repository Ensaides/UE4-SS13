// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "PDA.generated.h"

class AIDCard;
class ACartridge;

UCLASS()
class SPACESTATIONGAME_API APDA : public AItem
{
	GENERATED_BODY()

	// Sets default values for this actor's properties
	APDA(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = ID)
		uint8 AssignedJob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = ID)
		AIDCard* InsertedID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = ID)
		ACartridge* InsertedCartridge;
};
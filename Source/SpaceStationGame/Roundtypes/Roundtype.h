// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Roundtype.generated.h"

UCLASS()
class SPACESTATIONGAME_API ARoundtype : public AActor
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintNativeEvent, Category = Round)
		void InitializeRound();

	virtual void InitializeRound();
};
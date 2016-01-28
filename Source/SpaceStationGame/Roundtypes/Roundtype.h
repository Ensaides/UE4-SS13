// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RoundType.generated.h"

UCLASS()
class SPACESTATIONGAME_API ARoundType : public AActor
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintNativeEvent, Category = Round)
		void InitializeRound();

	UFUNCTION(BlueprintNativeEvent, Category = Round)
		void EndRound();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Round)
	FString RoundTypeName;
};
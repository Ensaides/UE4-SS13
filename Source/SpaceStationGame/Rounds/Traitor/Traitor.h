// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceStationGameGameMode.h"
#include "Traitor.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API ATraitor : public ASpaceStationGameGameMode
{
	GENERATED_UCLASS_BODY()
	
	virtual void StartRound() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
		int32 TraitorRatio;
};

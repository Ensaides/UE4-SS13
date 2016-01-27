// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RoundType.h"
#include "RoundTypeTraitor.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API ARoundTypeTraitor : public ARoundType
{
	GENERATED_UCLASS_BODY()
	
	virtual void InitializeRound_Implementation() override;
	
	int32 TraitorRatio;
};

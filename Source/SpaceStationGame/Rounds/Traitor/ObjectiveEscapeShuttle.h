// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Rounds/Objective.h"
#include "ObjectiveEscapeShuttle.generated.h"

/**
*
*/
UCLASS()
class SPACESTATIONGAME_API UObjectiveEscapeShuttle : public UObjective
{
	GENERATED_UCLASS_BODY()

	virtual FString GetObjectiveMessage_Implementation();


};

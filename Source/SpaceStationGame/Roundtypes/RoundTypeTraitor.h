// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RoundType.h"
#include "RoundTypeTraitor.generated.h"

#define TRAITOR_OBJECTIVES_MAX 128
#define TRAITOR_OBJECTIVES 1

/**
 * 
 */
UCLASS(Config=Game)
class SPACESTATIONGAME_API ARoundTypeTraitor : public ARoundType
{
	GENERATED_UCLASS_BODY()
	
	virtual void InitializeRound_Implementation() override;
	
	virtual void SetUpTraitorCharacter(class ASpaceStationGameCharacter* Character);

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = Traitor)
	int32 TraitorRatio;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = Traitor)
	int32 NumOfObjectives;

	// Objectives
	typedef void (ARoundTypeTraitor::*FunctionPtrType) (ASpaceStationGameCharacter*);

	FunctionPtrType TraitorObjectives[TRAITOR_OBJECTIVES_MAX];

	void AddObjective(int32 ObjectiveIndex, ASpaceStationGameCharacter* Character);

	void InitObjectives();

	// Objective functions

	// UPDATE THIS
	virtual void Objective_Assassinate(ASpaceStationGameCharacter* Character);
};

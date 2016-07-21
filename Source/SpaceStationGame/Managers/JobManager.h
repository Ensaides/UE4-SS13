// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager.h"
#include "Job.h"
#include "JobManager.generated.h"


/**
*
*/
UCLASS()
class SPACESTATIONGAME_API AJobManager : public AManager
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeJobs();

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (ToolTip = "Called during job initialization, implement this to spawn custom jobs", DisplayName = "Initialize Jobs"), Category = Jobs)
		void BP_InitializeJobs();

	// Array of jobs to be spawned
	UPROPERTY(EditDefaultsOnly, Category = Jobs)
		TArray<TSubclassOf<UJob>> JobClasses;

	UPROPERTY()
		TArray<UJob*> Jobs;
};

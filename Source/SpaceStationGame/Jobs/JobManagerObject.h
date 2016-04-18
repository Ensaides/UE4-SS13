// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManagerObject.h"

#include "JobObject.h"

#include "JobManagerObject.generated.h"

UCLASS()
class SPACESTATIONGAME_API UJobManagerObject : public UManagerObject
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Jobs)
		void SetUpJobs();

	UPROPERTY()
		TArray<TSubclassOf<UJobObject>> JobArray;

	UPROPERTY()
		TArray<FString> JobArrayNames;

	UFUNCTION(BlueprintCallable, Category = Jobs)
	void AddJob(TSubclassOf<UJobObject> NewJob, FString JobName);

	UFUNCTION(BlueprintCallable, Category = Jobs)
	bool GetJobExists(uint8 Job) { return JobArray.IsValidIndex(Job); };


	UFUNCTION(BlueprintCallable, Category = Jobs)
	uint8 GetJobIndex(TSubclassOf<UJobObject> Job);

	TArray<TSubclassOf<UJobObject>> GetJobArray() { return JobArray; };

	TArray<FString> GetJobNames() { return JobArrayNames; };

	TSubclassOf<UJobObject> GetJob(uint8 Job) { return JobArray[Job]; };
};
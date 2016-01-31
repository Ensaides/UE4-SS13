// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "JobManagerObject.generated.h"

class UJobObject;

UCLASS()
class SPACESTATIONGAME_API UJobManagerObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TArray<TSubclassOf<UJobObject>> JobArray;

	UPROPERTY()
		TArray<FString> JobArrayNames;

	UFUNCTION(BlueprintCallable, Category = Jobs)
	void AddJob(TSubclassOf<UJobObject> NewJob, FString JobName);

	UFUNCTION(BlueprintCallable, Category = Jobs)
	bool GetJobExists(uint8 Job) { return JobArray.IsValidIndex(Job); };

	TArray<TSubclassOf<UJobObject>> GetJobArray() { return JobArray; };

	TArray<FString> GetJobNames() { return JobArrayNames; };

	TSubclassOf<UJobObject> GetJob(uint8 Job) { return JobArray[Job]; };
};
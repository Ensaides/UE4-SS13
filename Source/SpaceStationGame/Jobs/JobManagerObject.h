// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "JobManagerObject.generated.h"


UCLASS()
class SPACESTATIONGAME_API UJobManagerObject : public UObject
{
	GENERATED_BODY()

	TArray<TSubclassOf<class UJobObject>> JobArray;

public:
	void AddJob(TSubclassOf<UJobObject> NewJob);

	bool GetJobExists(uint8 Job) { return JobArray.IsValidIndex(Job); };

	TSubclassOf<UJobObject> GetJob(uint8 Job) { return JobArray[Job]; };
};
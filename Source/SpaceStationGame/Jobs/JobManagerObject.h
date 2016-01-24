// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "JobManagerObject.generated.h"

// REEEEE, singletons!

UCLASS()
class SPACESTATIONGAME_API UJobManagerObject : public UObject
{
	GENERATED_BODY()

	TArray<TSubclassOf<class UJobObject>> JobArray;

public:
	void AddJob(TSubclassOf<UJobObject> NewJob);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "JobObject.h"
#include "JobManagerObject.generated.h"

// REEEEE, singletons!

UCLASS()
class SPACESTATIONGAME_API UJobManagerObject : public UObject
{
	GENERATED_BODY()

	UJobManagerObject();

	TArray<TSubclassOf<UJobObject>> JobArray;
};
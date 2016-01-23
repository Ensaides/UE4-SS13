// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Jobs.h"
#include "JobObject.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API UJobObject : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
		EJobs Job;

	UPROPERTY()
		FInventory StartingInventory;

	UPROPERTY()
		FAccess DefaultAccess;

public:
	UJobObject();

	static UJobObject* GetJobObjectFromJob(EJobs Job);
};

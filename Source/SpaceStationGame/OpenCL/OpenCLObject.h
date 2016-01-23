// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"

#include "CL/cl.h"

#include "OpenCLObject.generated.h"

/**
 * 
 */

UCLASS()
class SPACESTATIONGAME_API UOpenCLObject : public UObject
{
	GENERATED_BODY()

	void SetUpOpenCL();
	
	
};

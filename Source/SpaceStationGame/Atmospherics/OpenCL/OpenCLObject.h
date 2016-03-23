// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "CL/cl.h"

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

#include "OpenCLObject.generated.h"

/**
 * 
 */

UCLASS()
class SPACESTATIONGAME_API UOpenCLObject : public UObject
{
	GENERATED_BODY()

	void SetUpOpenCL();
	
public:
	UOpenCLObject(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize();

protected:
	bool bThreadRunning;

	std::thread OpenCLThread;

	std::vector<cl_platform_id> Platforms;
	std::vector<cl_device_id> Devices;
	
	cl_context Context;

	cl_program Program;

	void GetOpenCLData();
};

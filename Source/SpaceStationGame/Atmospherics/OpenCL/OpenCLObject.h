// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Tickable.h"
#include "OpenCL.h"

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

#include "OpenCLObject.generated.h"

class UAtmosVoxelManager;

/**
 * 
 */

UCLASS()
class SPACESTATIONGAME_API UOpenCLObject : public UObject
{
	GENERATED_BODY()

	void SetUpOpenCL();
	
	UAtmosVoxelManager* VoxelManager;

private:
	void CheckError(int Error);

public:
	UOpenCLObject(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize();

	virtual void Tick(float DeltaTime);

	virtual void BeginDestroy() override;

public:
	bool bThreadRunning;

	bool bComputingAtmospherics;

	std::thread OpenCLThread;

	std::vector<cl_platform_id> Platforms;
	cl_device_id Device;
	
	cl_context Context;
	cl_program Program;
	cl_kernel Kernel;

	void GetOpenCLData();
};

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

namespace UOpenCL
{
	enum AtmosWriteOperation
	{
		Add,
		Subtract,
		Set,
		SetAdditive, // If you want to voxel to gain gas pressure every frame
		SetSubtractive // If you want to voxel to remove gas pressure every frame
	};

	struct AtmosReadStruct
	{
		// Reads from the specified ID and writes to the pointed data
		int VoxelID;

		std::mutex* Mutex;
		bool* GasWriteCompleted;
		cl_float16* GasesToWrite;
	};

	struct AtmosWriteStruct
	{
		int VoxelID;

		AtmosWriteOperation WriteOperation;
		cl_float16 WriteOperand;
	};
}

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
	
	std::mutex ReadStructsLock;

	static std::vector<UOpenCL::AtmosReadStruct> ReadStructs;

	std::mutex WriteStructsLock;

	static std::vector<UOpenCL::AtmosWriteStruct> WriteStructs;

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

	std::atomic<bool> bCycleAtmos;

	std::thread OpenCLThread;

	std::vector<cl_platform_id> Platforms;
	cl_device_id Device;

	cl_mem InputAtmosVoxelsBuffer;
	cl_mem OutputAtmosVoxelsBuffer;
	cl_mem UpdateAtmosVoxelsBuffer;

	cl_event event;
	
	cl_command_queue CommandQueue;

	cl_context Context;
	cl_program Program;
	cl_kernel Kernel;

	void GetOpenCLData();
};

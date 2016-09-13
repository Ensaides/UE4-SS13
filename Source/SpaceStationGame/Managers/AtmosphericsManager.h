// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <array>

#include "OpenCL.h"

#include "Tickable.h"
#include "Manager.h"
#include "Atmos.h"
#include "AtmosphericsManager.generated.h"

enum AtmosphericsManagerState
{
	OpenCLInitialization,
	ComputingAtmospherics,
	TransferingData
};

/**
*	Manages the atmospherics voxels and uses OpenCL to calculate atmospheric changes
*/
UCLASS()
class SPACESTATIONGAME_API AAtmosphericsManager : public AManager
{
	GENERATED_UCLASS_BODY()

	/*
public:
	virtual void Initialize() override;
	virtual void CompleteInitialization() override {};
	virtual void Tick( float DeltaSeconds ) override;
	virtual void BeginDestroy() override;

public:
	// Voxels
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Voxels)
		int32 MaxVoxelCount;

	int32 VoxelCount;

	FAtmosVoxel* Voxels;

private:
	//				OpenCL state
	AtmosphericsManagerState State;

	void SetState(AtmosphericsManagerState NewState);

	// Sets up OpenCL and compiles the kernels
	void OnOpenCLInitialization();
	
	// Computes atmospherics and performs read operations on the OpenCL voxel buffer at the same time
	void OnComputingAtmospherics();

	// Transfers data from the output to the input, performs write operations on the input buffer
	void OnTransferingData();

	//				Threading members
	std::atomic<bool> bThreadRunning;
	std::atomic<bool> bComputingAtmospherics;

	// These are the input/output buffers, they are swapped after atmospherics are computed and assigned to the input/output pointers
	cl_mem Buffer1;
	cl_mem Buffer2;

	cl_mem* InputBuffer;
	cl_mem* OutputBuffer;

#ifdef UE_BUILD_DEBUG
	cl_mem DebugInfoBuffer;
#endif

	cl_event event;

	OpenCLProgram ComputeAtmospherics;
	OpenCLProgram CommitAtmosphericWrites;

#ifdef UE_BUILD_DEBUG
	OpenCLProgram GetDebugInfo;
#endif

	// OpenCL error checking
	static void CheckError(int Error);

	static void CheckBuildError(int Error, cl_program Program, cl_device_id* Device);

	// Thread function
	void GetOpenCLData();

private:
	std::atomic<bool> bComputeAtmospherics;

	// Last time stamp (in seconds) is used to calculate delta time
	double LastTimeStamp;
	float DeltaTime;
	std::mutex AtmosphericsMutex;

public:
	// Disables the frame limiter, unties the computations from the frame rate
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Computations)
		bool bBenchmarkMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Atmos)
		bool bPrintAtmos;

#ifdef UE_BUILD_DEBUG
private:
	float FrameRate;
	std::mutex FrameRateMutex;
#endif

private:
	std::vector<FAtmosVoxel> NewVoxels;
	std::mutex NewVoxelsMutex;

	std::vector<BufferReadStruct> BufferReads;
	std::mutex BufferReadsMutex;

	// One write struct for each operation
	std::vector<std::array<Atmospherics::AtmosVoxelWrite, 5>> BufferWrites;
	std::mutex BufferWritesMutex;

public:
	// TO DO: Add a blueprint version of this somehow
	void AddBufferRead(FAtmosVoxel* VoxelPtr, int32 VoxelIndex, std::mutex* DataMutex);

	void AddBufferWrite(int32 VoxelIndex, Atmospherics::AtmosWriteOperation Operation, cl_float16 Value);

	int32 AddVoxel();
	*/
};
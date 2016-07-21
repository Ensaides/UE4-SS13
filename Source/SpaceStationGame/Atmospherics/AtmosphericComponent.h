// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"

#include "AtmosVoxel.h"
#include <atomic>
#include <mutex>

#include "AtmosphericComponent.generated.h"

// Wrapper class for an atmospherics voxel
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACESTATIONGAME_API UAtmosphericComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAtmosphericComponent();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Called after the atmospherics are updated
	virtual void ReceiveAtmosUpdate();

	UFUNCTION(Client, Unreliable)
		void ClientReceiveAtmosUpdate(const FGases UpdatedGases);

	virtual void ClientReceiveAtmosUpdate_Implementation(const FGases UpdatedGases) { Gases = UpdatedGases; };

	// Should the atmospherics be updated every tick?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Atmospherics)
		bool bUpdateAtmospherics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Atmospherics)
		bool bReplicateAtmospherics;

private:
	// Atmos members
	FAtmosVoxel AtmosData;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gases)
		FGases StartingGases;

	// Client side members
	UPROPERTY(Replicated)
		FGases Gases;

	virtual void SetUpGases();

private:
	std::mutex AtmosDataMutex;

	int32 VoxelIndex;

	std::atomic<bool> bReceivedData;
};

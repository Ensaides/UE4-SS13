// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"

#include "SpaceStationGameGameState.h"
#include "SpaceStationGameServerState.h"
#include "AtmosphericsManager.h"
#include "UnrealNetwork.h"

#include "AtmosphericComponent.h"


// Sets default values for this component's properties
UAtmosphericComponent::UAtmosphericComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UAtmosphericComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UAtmosphericComponent, Gases, COND_OwnerOnly);
}

// Called every frame
void UAtmosphericComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	UWorld* const World = GetWorld();

	if (bUpdateAtmospherics && bReceivedData && World)
	{
		auto GameState = World->GetGameState<ASpaceStationGameGameState>();

		if (GameState)
		{
			auto AtmosphericsManager = Cast<AAtmosphericsManager>(GameState->GetServerState()->GetAtmosphericsManager());

			//AtmosphericsManager->AddBufferRead(&AtmosData, VoxelIndex, &AtmosDataMutex);

			bReceivedData = false;
		}
	}
}

void UAtmosphericComponent::ReceiveAtmosUpdate()
{
	if (bReplicateAtmospherics)
	{
		std::unique_lock<std::mutex> Lock(AtmosDataMutex);
			Gases = AtmosData.GetFGases();
	}

	bReceivedData = true;
}

void UAtmosphericComponent::SetUpGases()
{
	AtmosData.Voxel.Gases = StartingGases.ToFloat16();

	UWorld* const World = GetWorld();
	if (World)
	{
		auto GameState = World->GetGameState<ASpaceStationGameGameState>();
		if (GameState)
		{
			auto AtmosphericsManager = Cast<AAtmosphericsManager>(GameState->GetServerState()->GetAtmosphericsManager());

			//AtmosphericsManager->AddBufferWrite(AtmosData.Voxel.Index, AtmosWriteOperation::Set, StartingGases.ToFloat16());
		}
	}
}
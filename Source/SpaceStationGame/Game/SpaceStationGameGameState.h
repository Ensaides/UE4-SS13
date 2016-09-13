// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "SpaceStationGameGameState.generated.h"

class ASpaceStationGameServerState;
class AManager;

/**
 *
 */
UCLASS()
class SPACESTATIONGAME_API ASpaceStationGameGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

public:
	// Spawns the server state and manager objects
	virtual void BeginPlay() override;
	
	// Server state
public:
	UPROPERTY()
		ASpaceStationGameServerState* ServerState;

public:
	ASpaceStationGameServerState* GetServerState() { return ServerState; };

public:

	// Manager Objects

	// This should be overridden in BP to call initializers on new managers
	UFUNCTION(BlueprintImplementableEvent, meta = (ToolTip = "Called during manager initialization, implement this to call initializers on custom manager objects", DisplayName = "Initialize Managers"), Category = Managers)
		void BP_InitializeManagers();

	// Instance Manager
	UPROPERTY(EditDefaultsOnly, Category = Managers)
		TSubclassOf<class AInstanceManager> InstanceManagerClass;

	UPROPERTY(BlueprintReadOnly, Category = Managers)
		AManager* InstanceManager;

	AManager* GetInstanceManager() const { return InstanceManager; };
};

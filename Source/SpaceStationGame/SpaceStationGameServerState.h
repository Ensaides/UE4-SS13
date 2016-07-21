// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpaceStationGameServerState.generated.h"

class AManager;

UCLASS()
class SPACESTATIONGAME_API ASpaceStationGameServerState : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Calls the initializers on all of the manager objects
	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

public:
	// Manager Objects

	// This should be overridden in BP to call initializers on new managers
	UFUNCTION(BlueprintImplementableEvent, meta = (ToolTip = "Called during manager initialization, implement this to call initializers on custom manager objects", DisplayName = "Initialize Managers"), Category = Managers)
		void BP_InitializeManagers();

	// Atmospherics Manager
	UPROPERTY(EditDefaultsOnly, Category = Managers)
		TSubclassOf<class AAtmosphericsManager> AtmosphericsManagerClass;

	UPROPERTY(BlueprintReadOnly, Category = Managers)
		AManager* AtmosphericsManager;

	AManager* GetAtmosphericsManager() const { return AtmosphericsManager; };

	// Chat Manager
	UPROPERTY(EditDefaultsOnly, Category = Managers)
		TSubclassOf<class AChatManager> ChatManagerClass;

	UPROPERTY(BlueprintReadOnly, Category = Managers)
		AManager* ChatManager;

	AManager* GetChatManager() const { return ChatManager; };
	
	// Job Manager
	UPROPERTY(EditDefaultsOnly, Category = Managers)
		TSubclassOf<class AJobManager> JobManagerClass;

	UPROPERTY(BlueprintReadOnly, Category = Managers)
		AManager* JobManager;

	AManager* GetJobManager() const { return JobManager; };
};

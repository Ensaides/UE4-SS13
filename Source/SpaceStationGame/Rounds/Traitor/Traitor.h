// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceStationGameGameMode.h"
#include "SpaceStationGamePlayerController.h"
#include "Objective.h"
#include "Traitor.generated.h"


/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API ATraitor : public ASpaceStationGameGameMode
{
	GENERATED_UCLASS_BODY()
	
	virtual void StartRound() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
		int32 TraitorRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
		int32 MaxTraitors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
		int32 MinTraitors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traitor")
		TArray<ASpaceStationGamePlayerController*> Traitors;

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add the random traitor objectives to a player", DisplayName = "Setup Player Objectives"), Category = "Player")
		virtual void SetupPlayerObjectives(ASpaceStationGamePlayerController* Player);

	// The primary objectives, these should be things like assassination, stealing, etc...
	UFUNCTION(BlueprintCallable, Category = "Objectives")
		static TArray<TSubclassOf<UObjective>>& GetPrimaryObjectives()
		{
			static TArray<TSubclassOf<UObjective>> PrimaryObjectives;

			return PrimaryObjectives;
		}

	UFUNCTION(BlueprintCallable, Category = "Objectives")
		static void AddPrimaryObjective(TSubclassOf<UObjective> NewObjective)
		{
			GetPrimaryObjectives().Add(NewObjective);
		}

	UFUNCTION(BlueprintCallable, Category = "Objectives")
		static void RemovePrimaryObjective(TSubclassOf<UObjective> RemoveObjective)
		{
			GetPrimaryObjectives().Remove(RemoveObjective);
		}

	// The second objectives, these should be escape objectives
	UFUNCTION(BlueprintCallable, Category = "Objectives")
		static TArray<TSubclassOf<UObjective>>& GetSecondaryObjectives()
		{
			static TArray<TSubclassOf<UObjective>> SecondaryObjectives;

			return SecondaryObjectives;
		}

	UFUNCTION(BlueprintCallable, Category = "Objectives")
		static void AddSecondaryObjective(TSubclassOf<UObjective> NewObjective)
		{
			GetSecondaryObjectives().Add(NewObjective);
		}

	UFUNCTION(BlueprintCallable, Category = "Objectives")
		static void RemoveSecondaryObjective(TSubclassOf<UObjective> RemoveObjective)
		{
			GetSecondaryObjectives().Remove(RemoveObjective);
		}
};

struct TRAITOR_ADD_PRIMARY_OBJECTIVE
{
	TRAITOR_ADD_PRIMARY_OBJECTIVE(TSubclassOf<UObjective> NewObjective)
	{
		ATraitor::AddPrimaryObjective(NewObjective);
	}
};

struct TRAITOR_ADD_SECONDARY_OBJECTIVE
{
	TRAITOR_ADD_SECONDARY_OBJECTIVE(TSubclassOf<UObjective> NewObjective)
	{
		ATraitor::AddSecondaryObjective(NewObjective);
	}
};

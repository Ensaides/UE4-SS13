// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RecipeContainer.h"
#include "Reagents.h"
#include <map>
#include <unordered_map>
#include "SpaceStationGameServerState.generated.h"

UCLASS()
class SPACESTATIONGAME_API ASpaceStationGameServerState : public AActor
{
	GENERATED_BODY()

public:	
	ASpaceStationGameServerState(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	////Server stuff

	// MySQL stuff
	UPROPERTY()
	class UMySQLObject* MySQLObject;

	UPROPERTY()
	class UOpenCLObject* OpenCLObject;

public:
	void SetUpMySQLPlayerData(APlayerController* NewPlayer);

	uint8 GetMySQLPreferredJob(APlayerController* NewPlayer);

	uint32 GetMySQLPreferredAntagonistRoles(APlayerController* NewPlayer);

	// Recipe stuff

	UDataTable* RecipeLookupTable;

	UPROPERTY()
		URecipeContainer* RecipeContainer;

	std::pair<EReagents, uint32> GetReagentPair(FString Reagent, FString StringToInt);

	void SetupRecipes();

	bool GetRecipeExists(FRecipe InRecipe);

	FRecipe GetProductReagentFromRecipe(FRecipe InRecipe);
};

USTRUCT(Blueprintable)
struct FRecipeLookupTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "RO")
		FString Reagents;

	UPROPERTY(BlueprintReadOnly, Category = "RO")
		FString Products;
};
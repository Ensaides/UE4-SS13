#pragma once

#include "Object.h"
#include "Objective.generated.h"

// Keeps track of a player's objective and reports to the game mode
UCLASS(Abstract)
class UObjective : public UObject
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintNativeEvent, Category = "Objective")
		void InitializeObjective();

	UFUNCTION(BlueprintNativeEvent, Category = "Objective")
		FString GetObjectiveMessage();
};
#pragma once

#include "Object.h"
#include "Access.h"
#include "Inventory.h"
#include "Job.generated.h"

class AHuman;

UCLASS(Abstract)
class UJob : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Job)
		FAccess DefaultAccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Job)
		FString JobName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Job)
		FString JobMessage;

	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Called when a character spawns, used to spawn the starting inventory items and set them up for that character", DisplayName = "Construct Inventory"), Category = Job)
		void ConstructInventory(AHuman* OwningHuman);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Automatically set up and spawn the ID card from the job info", DisplayName = "Construct ID Card"), Category = Job)
		void ConstructIDCard(AHuman* OwningHuman);
};
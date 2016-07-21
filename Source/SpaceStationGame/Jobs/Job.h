#pragma once

#include "Object.h"
#include "Access.h"
#include "Inventory.h"
#include <functional>
#include "Job.generated.h"

UCLASS(Abstract)
class UJob : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Job)
		FAccess DefaultAccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Job)
		FString JobName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Job)
		FString JobMessage;

	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Called when a character spawns, used to spawn the starting inventory items and set them up for that character", DisplayName = "Construct Inventory"), Category = Job)
		void ConstructInventory();

	virtual void ConstructInventory_Implementation() {};
};
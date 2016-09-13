#pragma once

#include "Inventory.generated.h"

class AItem;

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		TArray<AItem*> Items;

	FInventory()
	{

	};
	
	FInventory(int32 Size)
	{
		Items.Init(NULL, Size);
	};
};
#pragma once

#include "Job.h"
#include "Assistant.generated.h"

UCLASS()
class UAssistant : public UJob
{
	GENERATED_UCLASS_BODY()

	virtual void ConstructInventory_Implementation(AHuman* OwningHuman) override;
};

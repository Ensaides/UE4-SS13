// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Manager.generated.h"

UCLASS(abstract)
class AManager : public AActor
{
	GENERATED_UCLASS_BODY()

	bool bCompletedInitialization;

public:
	// Usually called by the game state to initialize each manager object
	virtual void Initialize() { CompleteInitialization(); };

private:
	// Used by the object to complete initialization
	virtual void CompleteInitialization() { bCompletedInitialization = true; };
};
// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "SpaceStationGameGameMode.generated.h"

UCLASS(minimalapi)
class ASpaceStationGameGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASpaceStationGameGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	// Round stuff
	UPROPERTY(EditDefaultsOnly, Category = Round)
		float RoundStartDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Round)
		TArray<TSubclassOf<class ARoundType>> RoundTypeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Round)
		ARoundType* RoundType;

	bool bUseRoundStartReady;

	virtual void StartRound();

	virtual void HandleMatchHasStarted() override;

	virtual void HandleMatchHasEnded() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(class AController* NewPlayer) override;
};

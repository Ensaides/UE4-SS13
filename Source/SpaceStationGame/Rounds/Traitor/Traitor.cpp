// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "Human.h"
#include "Traitor.h"

ATraitor::ATraitor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TraitorRatio = 8;
}

void ATraitor::StartRound()
{
	Super::StartRound();

	for (auto Pawns = GetWorld()->GetPawnIterator(); Pawns; ++Pawns)
	{
		if (Cast<AHuman>(*Pawns))
		{
			
		}
	}
}

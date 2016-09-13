// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceStationGameCharacter.h"
#include "Human.generated.h"

// This describes the inventory slots
UENUM(BlueprintType)
enum class EHumanInventory : uint8
{
	Helmet 			UMETA(DisplayName = "Helmet"),
	Glasses 		UMETA(DisplayName = "Glasses"),
	Ear				UMETA(DisplayName = "Ear"),
	Clothes			UMETA(DisplayName = "Clothes"),
	ExClothes		UMETA(DisplayName = "External Clothes"),
	Hands			UMETA(DisplayName = "Hands"),	
	Shoes			UMETA(DisplayName = "Shoes"),
	SuitStorage		UMETA(DisplayName = "Suit Storage"),
	ID				UMETA(DisplayName = "ID"),
	Belt			UMETA(DisplayName = "Belt"),
	Backpack		UMETA(DisplayName = "Backpack"),
	RightHand		UMETA(DisplayName = "Right Hand"),
	LeftHand		UMETA(DisplayName = "Left Hand"),
	Pocket1			UMETA(DisplayName = "Pocket 1"),
	Pocket2			UMETA(DisplayName = "Pocket 2")
};

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API AHuman : public ASpaceStationGameCharacter
{
	GENERATED_UCLASS_BODY()
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Inventory.h"
#include "JobObject.generated.h"

USTRUCT(BlueprintType)
struct FAccess
{
	GENERATED_USTRUCT_BODY()

	//Security
	uint32 bBrig : 1;
	uint32 bWeaponPermit : 1;
	uint32 bSecurity : 1;
	uint32 bHoldingCells : 1;
	uint32 bArmory : 1;
	uint32 bForensics : 1;
	uint32 bCourtroom : 1;
	uint32 bHeadOfSecurity : 1;

	//Medical
	uint32 bMedical : 1;


	//General
	uint32 bKitchen : 1;
	uint32 bBar : 1;
	uint32 bHydroponics : 1;
	uint32 bCustodialCloset : 1;
	uint32 bChapelOffice : 1;
	uint32 bCrematorium : 1;
	uint32 bLibrary : 1;
	uint32 bTheatre : 1;
	uint32 bLawOffice : 1;
	uint32 bHonkAccess : 1;
	uint32 bSilentAccess : 1;
	uint32 bMaintenanceAccess : 1;
};

/**
 * 
 */
UCLASS(Blueprintable)
class SPACESTATIONGAME_API UJobObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FInventory StartingInventory;

	UPROPERTY()
		FAccess DefaultAccess;

	UJobObject();

	void InitializeJob();

	// Set up your inventory and initialize variables here
	UFUNCTION(BlueprintImplementableEvent, Category = Job)
		void SetUpJobObject(uint8 JobIndex);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void Add(AItem* Item, uint8 inIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Job)
		FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Job)
		FString JobMessage;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		UObject* GetOwner() { return GetOuter(); };

	UFUNCTION(BlueprintCallable, Category = Inventory)
		AItem* SpawnInventoryActor(TSubclassOf<class AItem> Class);
};
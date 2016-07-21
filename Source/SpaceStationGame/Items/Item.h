// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EState_InGame			UMETA(DisplayName = "In game"),
	EState_InInventory		UMETA(DisplayName = "In inventory"),
	EState_BeingDropped		UMETA(DisplayName = "Being dropped")
};

UCLASS()
class SPACESTATIONGAME_API AItem : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		UStaticMeshComponent* CustomDepthMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SetItemState, Category = Item)
		EItemState State;

	UFUNCTION(BlueprintNativeEvent, Category = State)
		void OnRep_SetItemState();

	/*
	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = Default, meta = (DisplayName = "Item: Used"))
		virtual void Use(APawn* Pawn);

	virtual void Use_Implementation(APawn* Pawn);

	virtual bool Use_Validate(APawn* Pawn) { return true; };

	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = Default, meta = (DisplayName = "Item: Dropped"))
		virtual void Drop(APawn* Pawn);

	virtual void Drop_Implementation(APawn* Pawn);

	virtual bool Drop_Validate(APawn* Pawn) { return true; };

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = GUI)
		class USlateBrushAsset* GetInventoryIcon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool bStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int32 MaxNumberOfStackItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Item)
		TArray<AItem*> ItemStack;

	// Use the hologram when dropping this item?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool bUseHologram;

	// If this is true, then render a client side hologram model
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
		bool bBeingDropped;

	// Only drop this item onto a grid?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool bUseGrid;

	// If this item fits onto a grid/tile, whats the size of the grid/tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FVector GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FVector PlaceOffset;

protected:

	UPROPERTY(EditDefaultsOnly, Category = Icon)
		class USlateBrushAsset* InventoryIcon;
	*/
};
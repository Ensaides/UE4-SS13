// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Inventory.h"
#include "SpaceStationGameCharacter.generated.h"

class UInputComponent;

// This character is the base for all characters. It can move, use, and it has an inventory
UCLASS(config=Game)
class ASpaceStationGameCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	// The name that shows up to people when the character speaks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Attributes")
		FString NickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Health")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Health")
		float MaxHealth;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	// Call this to kill this player. Only works on the server
	UFUNCTION(BlueprintCallable, Category = "Health")
		void Kill(AActor* Attacker);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKill, AActor*, Attacker);

	UPROPERTY(BlueprintAssignable)
		FKill OnKill;

	// Inventory members
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
		int32 InventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Inventory")
		FInventory Inventory;

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};


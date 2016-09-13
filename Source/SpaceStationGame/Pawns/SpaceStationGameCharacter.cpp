// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SpaceStationGame.h"
#include "SpaceStationGameCharacter.h"
#include "UnrealNetwork.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

ASpaceStationGameCharacter::ASpaceStationGameCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Replication
	bReplicates = true;
	bReplicateMovement = true;

	// Inventory
	InventorySize = 0;
}

void ASpaceStationGameCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Inventory
	DOREPLIFETIME_CONDITION(ASpaceStationGameCharacter, Inventory, COND_OwnerOnly);
}

void ASpaceStationGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ASpaceStationGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ASpaceStationGameCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASpaceStationGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ASpaceStationGameCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ASpaceStationGameCharacter::LookUpAtRate);
}

void ASpaceStationGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASpaceStationGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASpaceStationGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASpaceStationGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

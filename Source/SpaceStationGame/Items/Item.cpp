// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "Item.h"

#include "UnrealNetwork.h"

AItem::AItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	State = EItemState::EState_InGame;

	Scene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Scene"));
	RootComponent = Scene;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CustomDepthMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Custom Depth Mesh"));
	CustomDepthMesh->SetupAttachment(Mesh);

	CustomDepthMesh->bRenderCustomDepth = false;
	CustomDepthMesh->bRenderInMainPass = false;
	CustomDepthMesh->bCastDynamicShadow = false;
	CustomDepthMesh->bCastStaticShadow = false;
	CustomDepthMesh->bCastHiddenShadow = false;
	CustomDepthMesh->bCastVolumetricTranslucentShadow = false;
	CustomDepthMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/CustomDepthMat.CustomDepthMat'"));

	if (Material.Object != NULL && CustomDepthMesh->IsValidLowLevel())
	{
		for (int32 MatIndexInt = 0; MatIndexInt <= CustomDepthMesh->GetNumMaterials(); MatIndexInt++)
		{
			CustomDepthMesh->SetMaterial(MatIndexInt, (UMaterial*)Material.Object);
		}
	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bReplicateMovement = true;

	Mesh->SetIsReplicated(true);
	CustomDepthMesh->SetIsReplicated(true);

	PrimaryActorTick.bCanEverTick = true;

	/*
	bStackable = false;

	MaxNumberOfStackItems = 50;
	*/
}

void AItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, State);
}

void AItem::OnRep_SetItemState_Implementation()
{
	switch (State) {
	case EItemState::EState_InInventory:
		if (Mesh)
		{
			Mesh->UnregisterComponent(); // physical item has been picked up, destroy its visible component
		}

		if (CustomDepthMesh)
		{
			CustomDepthMesh->UnregisterComponent();
		}
		break;

	case EItemState::EState_InGame:
		if (Mesh)
		{
			Mesh->RegisterComponent();
		}

		if (CustomDepthMesh)
		{
			CustomDepthMesh->RegisterComponent();
		}
		break;

	case EItemState::EState_BeingDropped:
		break;

	default:
		break;
	}
}
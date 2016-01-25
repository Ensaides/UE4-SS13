// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "ChatMessageStruct.h"
#include "Reagents.h"
#include "UnrealNetwork.h"
#include "JobManagerObject.h"
#include "JobObject.h"
#include "InstancedItemContainer.h"
#include "SpaceStationGameGameMode.h"
#include "SpaceStationGamePlayerController.h"

#if UE_SERVER || UE_EDITOR
#include "SpaceStationGameServerState.h"
#endif

#include "SpaceStationGameGameState.h"

ASpaceStationGameGameState::ASpaceStationGameGameState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{	
	InstancedItemContainerClasses.Add(AInstancedItemContainer::StaticClass());

	JobManagerObject = ObjectInitializer.CreateDefaultSubobject<UJobManagerObject>(this, TEXT("Job Manager Object"));

	SetUpJobs();
}

void ASpaceStationGameGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpaceStationGameGameState, InstancedItemContainers);
	DOREPLIFETIME(ASpaceStationGameGameState, bDelayedStart);
}

void ASpaceStationGameGameState::BeginPlay()
{
	if (!HasAuthority())
	{
		UWorld* const World = GetWorld();
		for (auto Iter = InstancedItemContainerClasses.CreateIterator(); Iter; Iter++)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			AInstancedItemContainer* const NewContainer = World->SpawnActor<AInstancedItemContainer>(*Iter, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);

			InstancedItemContainers.Add(NewContainer);

			NewContainer->SetupInstances();
		}
	}
	else
	{
		UWorld* const World = GetWorld();
		bDelayedStart = World->GetAuthGameMode()->bDelayedStart;
	}

#if UE_SERVER || UE_EDITOR
	UWorld* const World = GetWorld();

	ServerState = World->SpawnActor<ASpaceStationGameServerState>(ASpaceStationGameServerState::StaticClass());
#endif

	Super::BeginPlay();
}

TSubclassOf<UJobObject> ASpaceStationGameGameState::GetJob(uint8 Job)
{
	if (JobManagerObject->GetJobExists(Job))
	{
		return JobManagerObject->GetJob(Job);
	}
	else
	{
		return UJobObject::StaticClass();
	}
}

void ASpaceStationGameGameState::StartMatchTimer(float TimerLength)
{
	GetWorldTimerManager().SetTimer(RoundStartTimerHandle, this, &ASpaceStationGameGameState::StartRound, TimerLength, false);
}

void ASpaceStationGameGameState::StartRound()
{
	Cast<ASpaceStationGameGameMode>(GetWorld()->GetAuthGameMode())->bDelayedStart = false;
}

AInstancedItemContainer* ASpaceStationGameGameState::GetContainerFromClass(UClass* InputClass)
{
	for (auto Iter = InstancedItemContainers.CreateIterator(); Iter; Iter++)
	{
		AInstancedItemContainer* IterPtr = *Iter;

		if (IterPtr->GetClass() == InputClass)
		{
			return IterPtr;
		}
	}

	return InstancedItemContainers[0];
}

void ASpaceStationGameGameState::AddChatMessage(const FString& Msg, FVector PlayerLocation, const FString& PlayerName)
{
	FChatMessageStruct NewMessage;

	NewMessage.ChatMsg = Msg;
	NewMessage.Location = PlayerLocation;
	NewMessage.PlayerName = PlayerName;

	int32 NewMessageIndex = ChatMessages.Add(&NewMessage);

	SendNewChatMessage(ChatMessages[NewMessageIndex]->ChatMsg, ChatMessages[NewMessageIndex]->PlayerName, ChatMessages[NewMessageIndex]->Location);
}

void ASpaceStationGameGameState::SendNewChatMessage_Implementation(const FString& Msg, const FString& PlayerName, FVector Location)
{
	for (TActorIterator<ASpaceStationGamePlayerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetPawn() && (ActorItr->GetPawn()->GetActorLocation() - Location).Size() < 500.f)
		{
			ActorItr->AddChatMessage(PlayerName + FString(" says, \"") + Msg + FString("\""), false);
		}
		else if (ActorItr->GetSpectatorPawn())
		{
			ActorItr->AddChatMessage(PlayerName + FString(" says, \"") + Msg + FString("\""), false);
		}
	}
}
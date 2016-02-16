// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "AntagonistRoles.h"
#include "ChatMessageStruct.h"
#include "SpaceStationGameCharacter.h"
#include "SpaceStationGamePlayerController.generated.h"

class AIDCard;
class AItem;

/**
*
*/
UCLASS()
class SPACESTATIONGAME_API ASpaceStationGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	void ResetOutline();

	FTimerHandle OutlineTimerHandle;

	TWeakObjectPtr<AItem> OutlineActor;

	TWeakObjectPtr<AIDCard> CardActor;

	//Sends the message to the server
	UFUNCTION(exec)
		void ChatFunc(const FString& InputString);

	UFUNCTION(reliable, server, WithValidation)
		void ServerChatFunc(const FString& InputString);

	bool ServerChatFunc_Validate(const FString& InputString) { return true; };

	void ServerChatFunc_Implementation(const FString& InputString);

	// Debug function to test death
//#if !UE_BUILD_SHIPPING
	UFUNCTION(exec)
		void Kill() { ServerKill(); };

	UFUNCTION(reliable, server, WithValidation)
		void ServerKill();

	bool ServerKill_Validate() { return true; };

	void ServerKill_Implementation() 
	{
		if (Cast<ASpaceStationGameCharacter>(GetPawn()) && Cast<ASpaceStationGameCharacter>(GetPawn())->bAlive) Cast<ASpaceStationGameCharacter>(GetPawn())->SetHealth(0);
	};
//#endif // !UE_BUILD_SHIPPING

	TArray<FClientChatMessageStruct> ChatMessages;

	FString GetChatName();

public:

	ASpaceStationGamePlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, server, unreliable, withvalidation, Category = Round)
		void ToggleRoundStartReady();

	bool ToggleRoundStartReady_Validate() { return true; };

	void ToggleRoundStartReady_Implementation();

	UFUNCTION(BlueprintCallable, server, unreliable, withvalidation, Category = Round)
		void JoinGame();

	bool JoinGame_Validate() { return true; };

	void JoinGame_Implementation();

	UFUNCTION(Client, Reliable)
		void HandleRoundStart();

	void HandleRoundStart_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = Default)
		void HandleRoundStart_BP();

	bool bHighlightUsableItems;

	/** True if the network time is valid. */
	bool IsNetworkTimeValid();

	UFUNCTION(client, reliable, withvalidation)
		void AddChatMessage(const FString& Msg, bool Radio);

	bool AddChatMessage_Validate(const FString& Msg, bool Radio) { return true; };

	void AddChatMessage_Implementation(const FString& Msg, bool Radio);

	UFUNCTION(BlueprintImplementableEvent, Category = Default)
		void AddChatMessageHud(FClientChatMessageStruct Msg);

	/** Gets the current system time in milliseconds */
	static int64 GetLocalTime();

	/** Gets the approximate current network time in milliseconds. */
	int64 GetNetworkTime();

	UFUNCTION(BlueprintCallable, Category = Chat)
		TArray<FClientChatMessageStruct> GetChatMessages();

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
		void Enter();

	// Round start preferences
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Player Data")
		bool bMySQLPlayerDataLoaded;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Player Data")
		FString LoadingState;

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Data")
		void OpenLoadingScreen();

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player Data")
		bool RoundStartReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player Data")
		uint8 StartingJob;

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = Job)
		void SetStartingJob(uint8 Job);

	bool SetStartingJob_Validate(uint8 Job) { return true; };

	void SetStartingJob_Implementation(uint8 Job) { StartingJob = Job; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Replication)
		FPreferredAntagonistRole PreferredAntagonistRole;

	UFUNCTION(Server, WithValidation, Reliable)
		void SetPreferredAntagonistRole(uint32 NewPreferredAntagonistRole);

	bool SetPreferredAntagonistRole_Validate(uint32 NewPreferredAntagonistRole) { return true; };

	void SetPreferredAntagonistRole_Implementation(uint32 NewPreferredAntagonistRole) { PreferredAntagonistRole = *(FPreferredAntagonistRole*)&NewPreferredAntagonistRole; };

protected:
	int64 timeServerTimeRequestWasPlaced;
	int64 timeOffsetFromServer;
	bool timeOffsetIsValid;

	/** Sent from a client to the server to get the server's system time */
	UFUNCTION(reliable, server, WithValidation)
		void ServerGetServerTime();

	void ServerGetServerTime_Implementation();

	bool ServerGetServerTime_Validate() { return true; };

	/** Sent from the server to a client to give them the server's system time */
	UFUNCTION(reliable, client)
		void ClientGetServerTime(int64 serverTime);

	void ClientGetServerTime_Implementation(int64 serverTime);
};

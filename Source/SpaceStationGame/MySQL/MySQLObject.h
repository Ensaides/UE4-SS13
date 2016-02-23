// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

#include "SpaceStationGamePlayerController.h"

#include "MySQLObject.generated.h"

namespace MySQL
{
	enum ThreadInputType
	{
		PlayerData,
		BanData
	};

	struct ThreadInputStruct
	{
		ThreadInputType InputType;

		FString Address;
		FString SteamID;
		ASpaceStationGamePlayerController* Player;
	};

	struct ThreadOutputStruct
	{
		uint8 PrefferedJob;
		uint32 PrefferedAntagonistRoles;
	};
}

/**
 * 
 */
UCLASS(config = Game)
class SPACESTATIONGAME_API UMySQLObject : public UObject
{
	GENERATED_BODY()

	void GetMySQLData();

	void RetryConnection();

	std::mutex MySQLLock;

	std::vector<MySQL::ThreadInputStruct> ThreadInput;

public:
	void Initialize();

	virtual void BeginDestroy() override;

	UPROPERTY(config)
		FString ServerODBCName;

	UPROPERTY(config)
		FString ServerUsername;

	UPROPERTY(config)
		FString ServerPort;

	UPROPERTY(config)
		FString ServerPassword;

	UPROPERTY(config)
		FString ServerDatabase;

	UPROPERTY(config)
		uint32 MySQLRetryDuration;

	void GetPlayerData(FString SteamID, ASpaceStationGamePlayerController* Player);

	void GetBanData(FString Address, FString UniqueId);

	const UWorld* World;

protected:

	std::thread MySQLThread;

	bool bThreadRunning;

	bool bConnectionActive;

private:
	void OpenConnection();

	void SetUpMySQLPlayerData(FString SteamID);

	uint8 GetMySQLPreferredJob(FString SteamID);

	uint32 GetMySQLPrefferedAntagonistRole(FString SteamID);

	void LoadBans();
};
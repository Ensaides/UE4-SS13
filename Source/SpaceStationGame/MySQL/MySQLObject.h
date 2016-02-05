// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include <memory>
#include <thread>
#include <mutex>
#include <vector>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"

#include "MySQLObject.generated.h"

struct ThreadInputStruct
{
	FString SteamID;
	class ASpaceStationGamePlayerController* Player;
};

struct ThreadOutputStruct
{
	uint8 PrefferedJob;
	uint32 PrefferedAntagonistRoles;
};

/**
 * 
 */
UCLASS(config = Game)
class SPACESTATIONGAME_API UMySQLObject : public UObject
{
	GENERATED_BODY()

	void GetMySQLData();

	void RetryConnection();

	std::mutex InputLock;

	std::mutex CharacterLock;

	std::vector<ThreadInputStruct> ThreadInput;

public:
	void Initialize();

	UPROPERTY()
		bool bConnectionActive;

	UPROPERTY(config)
		FString ServerUrl;

	UPROPERTY(config)
		FString ServerUsername;

	UPROPERTY(config)
		FString ServerPassword;

	UPROPERTY(config)
		FString ServerDatabase;

protected:

	std::thread* MySQLThread;

	sql::mysql::MySQL_Driver* driver;

	sql::Connection* con;

public:

	void OpenConnection();

	virtual void BeginDestroy() override;

	void SetUpMySQLPlayerData(FString SteamID);

	uint8 GetMySQLPreferredJob(FString SteamID);

	uint32 GetMySQLPrefferedAntagonistRole(FString SteamID);
};

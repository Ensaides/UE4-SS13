// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include <memory>
#include <thread>
#include <mutex>
#include <vector>

// OTL
#pragma warning(push)
#pragma warning(disable: 4946) // so that OTL doesn't clog up the compiler output. disabling warnings is probably a really bad idea
#define OTL_ODBC // Compile OTL 4.0/ODBC
// The following #define is required with MyODBC 5.1 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_UNICODE // Compile OTL with Unicode 
#define OTL_CPP_11_ON

#include "AllowWindowsPlatformTypes.h"
#include <otlv4.h> // include the OTL 4.0 header file
#include "HideWindowsPlatformTypes.h"
#pragma warning(pop)

//#include "mysql_connection.h"
//#include "mysql_driver.h"
//#include "mysql_error.h"

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

	std::mutex MySQLLock;

	std::vector<ThreadInputStruct> ThreadInput;

public:
	void Initialize();

	UPROPERTY()
		bool bConnectionActive;

	UPROPERTY(config)
		FString ServerAddress;

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

protected:

	std::thread MySQLThread;

	// OTL stuff
	//otl_connect* database;

	//odbc::otl_stream stream;

	// MySQL stuff
	/*sql::mysql::MySQL_Driver* driver;

	sql::Connection* con;*/

public:
	void OpenConnection();

	virtual void BeginDestroy() override;

	void SetUpMySQLPlayerData(FString SteamID);

	uint8 GetMySQLPreferredJob(FString SteamID);

	uint32 GetMySQLPrefferedAntagonistRole(FString SteamID);
};

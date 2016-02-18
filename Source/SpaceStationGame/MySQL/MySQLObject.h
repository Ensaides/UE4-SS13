// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

// OTL
#define OTL_ODBC // Compile OTL 4.0/ODBC
// The following #define is required with MyODBC 5.1 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_UNICODE // Compile OTL with Unicode 
#define OTL_CPP_11_ON

#if defined(__GNUC__)

namespace std {
	typedef unsigned short unicode_char;
	typedef basic_string<unicode_char> unicode_string;
}

#define OTL_UNICODE_CHAR_TYPE unicode_char
#define OTL_UNICODE_STRING_TYPE unicode_string

#else

#define OTL_UNICODE_CHAR_TYPE wchar_t
#define OTL_UNICODE_STRING_TYPE wstring
#endif

#pragma warning(push)
#pragma warning(disable: 4946) // Disable this warning so that OTL doesn't clog up the compiler output. disabling warnings is probably a really bad idea
#include "AllowWindowsPlatformTypes.h"
#include <otlv4.h> // include the OTL 4.0 header file
#include "HideWindowsPlatformTypes.h"
#pragma warning(pop)

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

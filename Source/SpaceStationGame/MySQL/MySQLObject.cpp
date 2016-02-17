// Fill out your copyright notice in the Description page of Project Settings.

// This is a server only class

#include "SpaceStationGame.h"

#include "MySQLObject.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>

#include "StringConv.h"
#include "StringHelpers.h"

#include "SpaceStationGamePlayerController.h"

#include "Online.h"
#include "OnlineSubsystem.h"

otl_connect database;

#if !UE_BUILD_SHIPPING
#define PRINT_ERRORS() \
	{ \
		SET_WARN_COLOR(COLOR_YELLOW); \
		std::string var_info(p.var_info); \
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error code:\t\t		%d"), p.code); \
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL variable error:\t\t	%s"), *StringHelpers::ConvertToFString(var_info)); \
		if (p.code == 32000) \
		{ \
			UE_LOG(SpaceStationGameLog, Warning, TEXT("Incompatible MySQL data types!")); \
		} \
		else UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info")); \
		CLEAR_WARN_COLOR(); \
		UE_LOG(SpaceStationGameLog, Warning, TEXT("Line Number:		%d"), __LINE__); \
		UE_LOG(SpaceStationGameLog, Warning, TEXT("Function:		%s"), *FString(__FUNCTION__)); \
	}
#else
	#define PRINT_ERRORS() \
	{ \
		SET_WARN_COLOR(COLOR_YELLOW); \
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error code:\t\t		%d"), p.code); \
		UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info")); \
		CLEAR_WARN_COLOR(); \
	}
#endif

void UMySQLObject::AddPlayerData(FString SteamID, ASpaceStationGamePlayerController* Player)
{
	ThreadInputStruct NewStruct;

	NewStruct.SteamID = SteamID;
	NewStruct.Player = Player;

	std::unique_lock<std::mutex> guard(MySQLLock);
	ThreadInput.push_back(NewStruct);
}

void UMySQLObject::Initialize()
{
	bThreadRunning = true;

	MySQLThread = std::thread(&UMySQLObject::GetMySQLData, this);
}

void UMySQLObject::GetMySQLData()
{
	// Start up the connection on this thread
	if (!bConnectionActive)
	{
		OpenConnection();
	}

	// Load the player data
	while (bThreadRunning)
	{
		{
			std::unique_lock<std::mutex> guard(MySQLLock);

			if (ThreadInput.size() > 0 && bConnectionActive)
			{
				ThreadInputStruct NewStruct = ThreadInput.back();
				ThreadInput.pop_back();
				guard.unlock();

				// Set the player preferences
				NewStruct.Player->SetStartingJob(GetMySQLPreferredJob(NewStruct.SteamID));
				NewStruct.Player->SetPreferredAntagonistRole(GetMySQLPrefferedAntagonistRole(NewStruct.SteamID));

				NewStruct.Player->bMySQLPlayerDataLoaded = true;
			}
			else if (!bConnectionActive)
			{
				guard.unlock();

				RetryConnection();
			}
		}

#if !UE_BUILD_SHIPPING
		//std::this_thread::sleep_for(std::chrono::seconds(4)); // Make it wait for a bit- for debug purposes
#endif// !UE_BUILD_SHIPPING

		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Run the thread at about 60 ticks/sec
	}
}

void UMySQLObject::RetryConnection()
{
	if (!bConnectionActive)
	{
		for (uint32 i = 0; i < 6; i++)
		{
			if (i == 6 && !bConnectionActive)
			{
				UE_LOG(SpaceStationGameLog, Fatal, TEXT("FATAL ERROR: MySQL failed to connect after 5 retries! Closing server"));
			}

			// Retry the connection after waiting
			std::this_thread::sleep_for(std::chrono::milliseconds( MySQLRetryDuration ));

			SET_WARN_COLOR(COLOR_YELLOW);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("Retrying connection to MySQL server..."));
			CLEAR_WARN_COLOR();

			OpenConnection();

			if (bConnectionActive)
			{
				return;
			}
		}
	}
	else return;
}

void UMySQLObject::OpenConnection()
{
	//Initialize OTL
	otl_connect::otl_initialize();

	if (!database.connected)
	{
		try
		{
			// Log into the database
			database.rlogon(StringHelpers::ConvertToString(ServerUsername + "/" + ServerPassword + "@" + ServerODBCName).c_str());

			otl_cursor::direct_exec
				(
					database,
					StringHelpers::ConvertToString("CREATE DATABASE IF NOT EXISTS " + ServerDatabase + " DEFAULT CHARACTER SET utf8;").c_str(),
					otl_exception::enabled
					);

			otl_cursor::direct_exec
				(
					database,
					StringHelpers::ConvertToString("USE " + ServerDatabase + ";").c_str(),
					otl_exception::enabled
					);

			// Player table
			otl_cursor::direct_exec
				(
					database,
					StringHelpers::ConvertToString("CREATE TABLE IF NOT EXISTS `" + ServerDatabase + "`.`playerpreferences` ("
					"`steamid` BIGINT(20) UNSIGNED NOT NULL,"
					"`preferredjob` TINYINT(3) UNSIGNED NOT NULL,"
					"`preferredantagonistroles` INT(11) UNSIGNED NOT NULL,"
					"PRIMARY KEY (`steamid`))"
					"DEFAULT CHARACTER SET = utf8;").c_str(),
					otl_exception::enabled
					);

			// Deaths table
			otl_cursor::direct_exec
				(
					database,
					StringHelpers::ConvertToString("CREATE TABLE IF NOT EXISTS `" + ServerDatabase + "`.`death` ("
						"`steamid` BIGINT(20) UNSIGNED NOT NULL,"
						"`coord` TEXT NOT NULL COMMENT 'X, Y, Z POD',"
						"`job` TEXT NOT NULL,"
						"`name` TEXT NOT NULL,"
						"`laname` TEXT NOT NULL COMMENT 'Last attacker name',"
						"`laid` TEXT NOT NULL COMMENT 'Last attacker steam id',"
						"`gender` TEXT NOT NULL,"
						"`bruteloss` INT(11) NOT NULL,"
						"PRIMARY KEY (`steamid`))"
						"DEFAULT CHARACTER SET = utf8;").c_str(),
					otl_exception::enabled
					);

			// Bans table
			otl_cursor::direct_exec
				(
					database,
					StringHelpers::ConvertToString("CREATE TABLE IF NOT EXISTS `" + ServerDatabase + "`.`ban` ("
						"`steamid` BIGINT(20) UNSIGNED NOT NULL,"
						"`banningid` BIGINT(20) UNSIGNED NOT NULL COMMENT 'The player that banned this player'"
						"`banlength` INT(11) SIGNED NOT NULL,"
						"`bandate` BIGINT(20) NOT NULL COMMENT 'Ban date in UTC time',"
						"`bancomment` TEXT,"
						"`ipaddress` TEXT NOT NULL,"
						"PRIMARY KEY (`steamid`))"
						"DEFAULT CHARACTER SET = utf8;").c_str(),
					otl_exception::enabled
					);

		}
		catch (otl_exception& p)
		{
			PRINT_ERRORS();

			bConnectionActive = false;

			return;
		}
	}
	else
	{
		bConnectionActive = true;
	}

	SET_WARN_COLOR(COLOR_CYAN);
	UE_LOG(SpaceStationGameLog, Log, TEXT("Connected to MySQL server on %s"), *ServerODBCName);
	CLEAR_WARN_COLOR();

	bConnectionActive = true;
}

void UMySQLObject::SetUpMySQLPlayerData(FString SteamID)
{
	if (bConnectionActive)
	{
		try
		{
			otl_cursor::direct_exec
				(
					database,
					("USE " + StringHelpers::ConvertToString(ServerDatabase)).c_str(),
					otl_exception::enabled
				);

			IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

			if (OnlineSub)
			{
				SET_WARN_COLOR(COLOR_CYAN);
				UE_LOG(SpaceStationGameLog, Log, TEXT("Steam subsystem initialized"));
				CLEAR_WARN_COLOR();

				// 19 is the assistant id
				otl_cursor::direct_exec
					(
						database,
						StringHelpers::ConvertToString("INSERT IGNORE INTO playerpreferences (steamid, preferredjob) VALUES (" + SteamID + ", 19);").c_str(),
						otl_exception::enabled
					);
			}
			else
			{
				SET_WARN_COLOR(COLOR_YELLOW);
				UE_LOG(SpaceStationGameLog, Warning, TEXT("Steam subsystem failed to initialize!"));
				CLEAR_WARN_COLOR();

#if !UE_BUILD_SHIPPING
				otl_cursor::direct_exec
					(
						database,
						"INSERT IGNORE INTO playerpreferences (steamid, preferredjob) VALUES (76561198004815982, 19);", // Use my steam id for debug
						otl_exception::enabled
					);
#endif // !UE_BUILD_SHIPPING
			}
		}
		catch (otl_exception& p)
		{
			PRINT_ERRORS();

			bConnectionActive = false;

			return;
		}
	}
}

uint8 UMySQLObject::GetMySQLPreferredJob(FString SteamID)
{
	if (bConnectionActive)
	{
		try
		{
			otl_cursor::direct_exec
				(
					database,
					StringHelpers::ConvertToString("USE " + ServerDatabase + ";").c_str(),
					otl_exception::enabled
				);

			IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

			if (OnlineSub)
			{
				otl_stream i(50000, // buffer size
					StringHelpers::ConvertToString("SELECT preferredjob FROM playerpreferences WHERE steamid LIKE " + SteamID).c_str(),
					database
					);

				int ReturnValue;

				while (!i.eof())
				{
					i>>ReturnValue;

					return ReturnValue;
				}
			}
#if !UE_BUILD_SHIPPING
			else
			{
				otl_stream i(50000, // buffer size
					"SELECT preferredjob FROM playerpreferences WHERE steamid LIKE 76561198004815982",
					database
					);

				int ReturnValue;

				while (!i.eof())
				{
					i>>ReturnValue;

					return ReturnValue;
				}
			}
#else // UE_BUILD_SHIPPING
			else
			{
				SET_WARN_COLOR(COLOR_YELLOW);
				UE_LOG(SpaceStationGameLog, Warning, TEXT("Steam subsystem failed to initialize!"));
				CLEAR_WARN_COLOR();
			}
#endif
		}
		catch (otl_exception& p)
		{
			PRINT_ERRORS();

			bConnectionActive = false;
		}
	}
	return 19;
}

uint32 UMySQLObject::GetMySQLPrefferedAntagonistRole(FString SteamID)
{
	if (bConnectionActive)
	{
		try
		{
		otl_cursor::direct_exec
			(
				database,
				StringHelpers::ConvertToString("USE " + ServerDatabase + ";").c_str(),
				otl_exception::enabled
				);

		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

		FString Statement = "SELECT preferredantagonistroles FROM playerpreferences WHERE steamid LIKE " + SteamID;

		if (OnlineSub)
		{
			otl_stream i(50000, // buffer size
				StringHelpers::ConvertToString(Statement).c_str(),
				database
				);

			int ReturnValue;

			while (!i.eof())
			{
				i >> ReturnValue;

				return ReturnValue;
			}
		}
#if !UE_BUILD_SHIPPING
		else
		{
			otl_stream i(50000, // buffer size
				"SELECT preferredantagonistroles FROM playerpreferences WHERE steamid LIKE 76561198004815982",
				database
				);

			int ReturnValue;

			while (!i.eof())
			{
				i >> ReturnValue;

				return ReturnValue;
			}
		}
#else // UE_BUILD_SHIPPING
		else
		{
			SET_WARN_COLOR(COLOR_YELLOW);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("Steam subsystem failed to initialize!"));
			CLEAR_WARN_COLOR();
		}
#endif
		}
		catch (otl_exception& p)
		{
			PRINT_ERRORS();

			bConnectionActive = false;
		}
	}
	return 0;
}

void UMySQLObject::BeginDestroy()
{
	Super::BeginDestroy();

	bThreadRunning = false;

	if (bConnectionActive)
	{
		bConnectionActive = false;
	}

	if (MySQLThread.joinable()) MySQLThread.join();

	database.logoff();
}
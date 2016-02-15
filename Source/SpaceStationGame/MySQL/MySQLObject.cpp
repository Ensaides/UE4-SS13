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
		if (ThreadInput.size() > 0 && bConnectionActive)
		{
			ThreadOutputStruct Output;

			Output.PrefferedJob = GetMySQLPreferredJob(ThreadInput.back().SteamID);

			Output.PrefferedAntagonistRoles = GetMySQLPrefferedAntagonistRole(ThreadInput.back().SteamID);

			std::unique_lock<std::mutex> guard(MySQLLock);
			ThreadInput.back().Player->SetStartingJob(Output.PrefferedJob);
			ThreadInput.back().Player->SetPreferredAntagonistRole(Output.PrefferedAntagonistRoles);
			guard.unlock();

#if !UE_BUILD_SHIPPING
			std::this_thread::sleep_for(std::chrono::seconds(1)); // Make it wait for a bit- for debug purposes
#endif// !UE_BUILD_SHIPPING
			
			guard.lock();
			ThreadInput.back().Player->bMySQLPlayerDataLoaded = true;

			ThreadInput.pop_back();
		}
		else if (!bConnectionActive)
		{
			RetryConnection();
		}
		//else
		//{
			std::this_thread::sleep_for(std::chrono::milliseconds(32));
		//}
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

	try
	{
		std::string LoginString;

		LoginString = StringHelpers::ConvertToString(ServerUsername) + "/" + StringHelpers::ConvertToString(ServerPassword) + "@" + StringHelpers::ConvertToString(ServerODBCName);

		database.rlogon(LoginString.c_str());

		otl_cursor::direct_exec
			(
				database,
				("CREATE DATABASE IF NOT EXISTS " + StringHelpers::ConvertToString(ServerDatabase)).c_str(),
				otl_exception::enabled
			);

		otl_cursor::direct_exec
			(
				database,
				("USE " + StringHelpers::ConvertToString(ServerDatabase)).c_str(),
				otl_exception::enabled
			);

		otl_cursor::direct_exec
			(
				database,
				"CREATE TABLE IF NOT EXISTS `players` ("
				"`steamid` BIGINT(20) UNSIGNED NOT NULL,"
				"`preferredjob` TINYINT(3) UNSIGNED NOT NULL,"
				"`preferredantagonistroles` BIT(32) NOT NULL,"
				"PRIMARY KEY (`steamid`));",
				otl_exception::enabled
			);


	}
	catch (otl_exception& p)
	{
		SET_WARN_COLOR(COLOR_YELLOW);
		// OTL is so verbose!
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error code:\t\t		%d"), p.code);
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL state:\t\t\t			" + p.sqlstate));
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error message:\t\t		" + p.msg));
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad statement:\t\t		" + p.stm_text));
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad variable:\t\t		" + p.var_info));
		UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
		CLEAR_WARN_COLOR();

		bConnectionActive = false;

		return;
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

				FString Statement = "INSERT IGNORE INTO players (steamid, preferredjob) VALUES (" + SteamID + ", 19);";

				// 19 is the assistant id
				otl_cursor::direct_exec
					(
						database,
						StringHelpers::ConvertToString(Statement).c_str(),
						otl_exception::enabled
					);
			}
			else
			{
				SET_WARN_COLOR(COLOR_YELLOW);
				UE_LOG(SpaceStationGameLog, Warning, TEXT("Steam subsystem failed to initialize!"));
				CLEAR_WARN_COLOR();

#if !UE_BUILD_SHIPPING
				//My steam id
				otl_cursor::direct_exec
					(
						database,
						"INSERT IGNORE INTO players (steamid, preferredjob) VALUES (76561198004815982, 19);",
						otl_exception::enabled
					);
#endif // !UE_BUILD_SHIPPING
			}
		}
		catch (otl_exception& p)
		{
			SET_WARN_COLOR(COLOR_YELLOW);
			// OTL is so verbose!
			UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error code:\t\t		%d"), p.code);
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL state:\t\t\t			" + p.sqlstate));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error message:\t\t		" + p.msg));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad statement:\t\t		" + p.stm_text));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad variable:\t\t		" + p.var_info));
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
			CLEAR_WARN_COLOR();

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
					("USE " + StringHelpers::ConvertToString(ServerDatabase)).c_str(),
					otl_exception::enabled
				);

			IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

			FString Statement = "SELECT preferredjob FROM players WHERE steamid LIKE " + SteamID;

			if (OnlineSub)
			{
				otl_stream i(50000, // buffer size
					StringHelpers::ConvertToString(Statement).c_str(),
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
					"SELECT preferredjob FROM players WHERE steamid LIKE 76561198004815982",
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
			SET_WARN_COLOR(COLOR_YELLOW);
			// OTL is so verbose!
			UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error code:\t\t		%d"), p.code);
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL state:\t\t\t			" + p.sqlstate));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error message:\t\t		" + p.msg));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad statement:\t\t		" + p.stm_text));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad variable:\t\t		" + p.var_info));
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
			CLEAR_WARN_COLOR();

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
				("USE " + StringHelpers::ConvertToString(ServerDatabase)).c_str(),
				otl_exception::enabled
				);

		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

		FString Statement = "SELECT preferredantagonistroles FROM players WHERE steamid LIKE " + SteamID;

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
				"SELECT preferredantagonistroles FROM players WHERE steamid LIKE 76561198004815982",
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
			SET_WARN_COLOR(COLOR_YELLOW);
			// OTL is so verbose!
			UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error code:\t\t		%d"), p.code);
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL state:\t\t\t			" + p.sqlstate));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error message:\t\t		" + p.msg));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad statement:\t\t		" + p.stm_text));
			//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad variable:\t\t		" + p.var_info));
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
			CLEAR_WARN_COLOR();

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

	MySQLThread.join();

	database.logoff();
}
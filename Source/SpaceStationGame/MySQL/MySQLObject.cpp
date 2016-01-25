// Fill out your copyright notice in the Description page of Project Settings.

// This is a server only class

#include "SpaceStationGame.h"

#include "MySQLObject.h"

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "StringConv.h"
#include "StringHelpers.h"

#include "Online.h"
#include "OnlineSubsystem.h"

void UMySQLObject::OpenConnection()
{
	try
	{
		driver = sql::mysql::get_mysql_driver_instance();

		con = driver->connect(StringHelpers::ConvertToString(ServerUrl), StringHelpers::ConvertToString(ServerUsername), StringHelpers::ConvertToString(ServerPassword));

		sql::Statement* stmt(con->createStatement());

		stmt->execute("CREATE DATABASE IF NOT EXISTS " + StringHelpers::ConvertToString(ServerDatabase));

		stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

		stmt->execute("CREATE TABLE IF NOT EXISTS `players` ("
			"`steamid` BIGINT(20) UNSIGNED NOT NULL,"
			"`preferredjob` TINYINT(3) UNSIGNED NOT NULL,"
			"`preferredantagonistroles` BIT(32) NOT NULL,"
			"PRIMARY KEY (`steamid`));");

		delete stmt;
	}
	catch (sql::SQLException &e)
	{
		SET_WARN_COLOR(COLOR_YELLOW);
		UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
		UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
		CLEAR_WARN_COLOR();

		bConnectionActive = false;

		return;
	}

	SET_WARN_COLOR(COLOR_CYAN);
	UE_LOG(SpaceStationGameLog, Log, TEXT("Connected to MySQL server on %s"), *ServerUrl);
	CLEAR_WARN_COLOR();

	bConnectionActive = true;
}

void UMySQLObject::SetUpMySQLPlayerData(FString SteamID)
{
	if (bConnectionActive)
	{
		try
		{
			sql::Statement* stmt(con->createStatement());

			stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

			IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

			if (OnlineSub)
			{
				SET_WARN_COLOR(COLOR_CYAN);
				UE_LOG(SpaceStationGameLog, Log, TEXT("Steam subsystem initialized"));
				CLEAR_WARN_COLOR();

				// 19 is the assistant id
				stmt->execute("INSERT IGNORE INTO players (steamid, preferredjob) VALUES (" + StringHelpers::ConvertToString(*SteamID) + ", 19);");
			}
			else
			{
				SET_WARN_COLOR(COLOR_YELLOW);
				UE_LOG(SpaceStationGameLog, Warning, TEXT("Steam subsystem failed to initialize!"));
				CLEAR_WARN_COLOR();

				//My steam id
				stmt->execute("INSERT IGNORE INTO players (steamid, preferredjob) VALUES (76561198004815982, 19);");
			}

			delete stmt;
		}
		catch (sql::SQLException &e)
		{
			SET_WARN_COLOR(COLOR_YELLOW);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
			CLEAR_WARN_COLOR();
		}
	}
}

uint8 UMySQLObject::GetMySQLPreferredJob(FString SteamID)
{
	if (bConnectionActive)
	{
		try
		{
			sql::Statement* stmt(con->createStatement());

			sql::ResultSet* res;

			IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

			stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

			if (OnlineSub)
			{
				res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE " + StringHelpers::ConvertToString(*SteamID));
			}
			else
			{
				res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE 76561198004815982");
			}

			//delete stmt;
			if (res->next())
			{
				return res->getUInt("preferredjob");
			}
			else return 19;
		}
		catch (sql::SQLException &e)
		{
			SET_WARN_COLOR(COLOR_YELLOW);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
			CLEAR_WARN_COLOR();
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
			sql::Statement* stmt(con->createStatement());

			sql::ResultSet* res;

			IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

			stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

			if (OnlineSub)
			{
				res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE " + StringHelpers::ConvertToString(*SteamID));
			}
			else
			{
				res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE 76561198004815982");
			}

			//delete stmt;
			if (res->next())
			{
				return res->getUInt("preferredantagonistroles");
			}
			else return 0;
		}
		catch (sql::SQLException &e)
		{
			SET_WARN_COLOR(COLOR_YELLOW);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
			UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
			CLEAR_WARN_COLOR();
		}
	}
	return 0;
}

void UMySQLObject::BeginDestroy()
{
	if (bConnectionActive)
	{
		con->close();

		con = nullptr;

		bConnectionActive = false;
	}

	Super::BeginDestroy();
}
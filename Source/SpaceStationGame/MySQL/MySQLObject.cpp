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

// Lock/unlock crap
#define GUARD_LOCK() \
	std::unique_lock<std::mutex> guard(MySQLLock);

#define GUARD_UNLOCK() \
	guard.unlock();

void UMySQLObject::Initialize()
{
	MySQLThread = std::thread(&UMySQLObject::GetMySQLData, this);
}

void UMySQLObject::GetMySQLData()
{
	// Start up the connection on this thread
	if (!bConnectionActive)
	{
		OpenConnection();
	}

	while (ThreadInput.size() > 0 && bConnectionActive)
	{
		ThreadOutputStruct Output;

		Output.PrefferedJob = GetMySQLPreferredJob(ThreadInput.back().SteamID);

		Output.PrefferedAntagonistRoles = GetMySQLPrefferedAntagonistRole(ThreadInput.back().SteamID);

		GUARD_LOCK();
		ThreadInput.back().Player->SetStartingJob(Output.PrefferedJob);
		ThreadInput.back().Player->SetPreferredAntagonistRole(Output.PrefferedAntagonistRoles);

		ThreadInput.pop_back();
	}

	while (!bConnectionActive)
	{	
		RetryConnection();
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
				GUARD_LOCK();
				UE_LOG(SpaceStationGameLog, Fatal, TEXT("FATAL ERROR: MySQL failed to connect after 5 retries! Closing server"));
			}

			// Retry the connection after waiting
			std::this_thread::sleep_for(std::chrono::milliseconds( MySQLRetryDuration ));

			GUARD_LOCK();
			SET_WARN_COLOR(COLOR_YELLOW);
			UE_LOG(SpaceStationGameLog, Warning, TEXT("Retrying connection to MySQL server..."));
			CLEAR_WARN_COLOR();
			GUARD_UNLOCK();


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

		//LoginString = "Server = " + StringHelpers::ConvertToString(ServerAddress) 
		//			+ "; Port = " + StringHelpers::ConvertToString(ServerPort) 
		//			+ "; Uid = " + StringHelpers::ConvertToString(ServerUsername) 
		//			+ "; Pwd = " + StringHelpers::ConvertToString(ServerPassword) + ";";

		//LoginString = "Driver = { MySQL ODBC 5.2 UNICODE Driver }; Server = " + StringHelpers::ConvertToString(ServerAddress)
		//	+ "; Port = " + StringHelpers::ConvertToString(ServerPort)
		//	+ "; User = " + StringHelpers::ConvertToString(ServerUsername)
		//	+ "; Password = " + StringHelpers::ConvertToString(ServerPassword) + "; Option = 3;";

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
		GUARD_LOCK();
		SET_WARN_COLOR(COLOR_YELLOW);
		// OTL is so verbose!
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error code:\t\t		%d"), p.code);
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL state:\t\t\t			" + p.sqlstate));
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL error message:\t\t		" + p.msg));
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad statement:\t\t		" + p.stm_text));
		//UE_LOG(SpaceStationGameLog, Warning, TEXT("OTL MySQL bad variable:\t\t		" + p.var_info));
		UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
		CLEAR_WARN_COLOR();
		GUARD_UNLOCK();

		return;
	}

	//// DEPRECATED MYSQL CODE
	//try
	//{
	//	driver = sql::mysql::get_mysql_driver_instance();

	//	con = driver->connect(StringHelpers::ConvertToString(ServerUrl), StringHelpers::ConvertToString(ServerUsername), StringHelpers::ConvertToString(ServerPassword));

	//	sql::Statement* stmt(con->createStatement());

	//	stmt->execute("CREATE DATABASE IF NOT EXISTS " + StringHelpers::ConvertToString(ServerDatabase));

	//	stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

	//	stmt->execute("CREATE TABLE IF NOT EXISTS `players` ("
	//		"`steamid` BIGINT(20) UNSIGNED NOT NULL,"
	//		"`preferredjob` TINYINT(3) UNSIGNED NOT NULL,"
	//		"`preferredantagonistroles` BIT(32) NOT NULL,"
	//		"PRIMARY KEY (`steamid`));");

	//	delete stmt;
	//}
	//catch (sql::SQLException &e)
	//{
	//	GUARD_LOCK();
	//	SET_WARN_COLOR(COLOR_YELLOW);
	//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
	//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
	//	CLEAR_WARN_COLOR();
	//	GUARD_UNLOCK();

	//	bConnectionActive = false;

	//	return;
	//}

	GUARD_LOCK();
	SET_WARN_COLOR(COLOR_CYAN);
	UE_LOG(SpaceStationGameLog, Log, TEXT("Connected to MySQL server on %s"), *ServerODBCName);
	CLEAR_WARN_COLOR();
	GUARD_UNLOCK();

	bConnectionActive = true;
}

void UMySQLObject::SetUpMySQLPlayerData(FString SteamID)
{
	if (bConnectionActive)
	{
		//try
		//{
		//	sql::Statement* stmt(con->createStatement());

		//	stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

		//	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

		//	if (OnlineSub)
		//	{
		//		GUARD_LOCK();
		//		SET_WARN_COLOR(COLOR_CYAN);
		//		UE_LOG(SpaceStationGameLog, Log, TEXT("Steam subsystem initialized"));
		//		CLEAR_WARN_COLOR();
		//		GUARD_UNLOCK();

		//		// 19 is the assistant id
		//		stmt->execute("INSERT IGNORE INTO players (steamid, preferredjob) VALUES (" + StringHelpers::ConvertToString(*SteamID) + ", 19);");
		//	}
		//	else
		//	{
		//		GUARD_LOCK();
		//		SET_WARN_COLOR(COLOR_YELLOW);
		//		UE_LOG(SpaceStationGameLog, Warning, TEXT("Steam subsystem failed to initialize!"));
		//		CLEAR_WARN_COLOR();
		//		GUARD_UNLOCK();

		//		//My steam id
		//		stmt->execute("INSERT IGNORE INTO players (steamid, preferredjob) VALUES (76561198004815982, 19);");
		//	}

		//	delete stmt;
		//}
		//catch (sql::SQLException &e)
		//{
		//	GUARD_LOCK();
		//	SET_WARN_COLOR(COLOR_YELLOW);
		//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
		//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
		//	CLEAR_WARN_COLOR();
		//	GUARD_UNLOCK();

		//	bConnectionActive = false;
		//}
	}
}

uint8 UMySQLObject::GetMySQLPreferredJob(FString SteamID)
{
	if (bConnectionActive)
	{
		//try
		//{
		//	sql::Statement* stmt(con->createStatement());

		//	sql::ResultSet* res;

		//	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

		//	stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

		//	if (OnlineSub)
		//	{
		//		res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE " + StringHelpers::ConvertToString(*SteamID));
		//	}
		//	else
		//	{
		//		res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE 76561198004815982");
		//	}

		//	//delete stmt;
		//	if (res->next())
		//	{
		//		return res->getUInt("preferredjob");
		//	}
		//	else return 19;
		//}
		//catch (sql::SQLException &e)
		//{
		//	GUARD_LOCK();
		//	SET_WARN_COLOR(COLOR_YELLOW);
		//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
		//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
		//	CLEAR_WARN_COLOR();
		//	GUARD_UNLOCK();

		//	bConnectionActive = false;
		//}
	}
	return 19;
}

uint32 UMySQLObject::GetMySQLPrefferedAntagonistRole(FString SteamID)
{
	if (bConnectionActive)
	{
		//try
		//{
		//	sql::Statement* stmt(con->createStatement());

		//	sql::ResultSet* res;

		//	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("Steam");

		//	stmt->execute("USE " + StringHelpers::ConvertToString(ServerDatabase));

		//	if (OnlineSub)
		//	{
		//		res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE " + StringHelpers::ConvertToString(*SteamID));
		//	}
		//	else
		//	{
		//		res = stmt->executeQuery("SELECT * FROM players WHERE steamid LIKE 76561198004815982");
		//	}

		//	//delete stmt;
		//	if (res->next())
		//	{
		//		return res->getUInt("preferredantagonistroles");
		//	}
		//	else return 0;
		//}
		//catch (sql::SQLException &e)
		//{
		//	GUARD_LOCK();
		//	SET_WARN_COLOR(COLOR_YELLOW);
		//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL error code: %d"), e.getErrorCode());
		//	UE_LOG(SpaceStationGameLog, Warning, TEXT("MySQL failed to connect, please check your mysql server info"));
		//	CLEAR_WARN_COLOR();
		//	GUARD_UNLOCK();

		//	bConnectionActive = false;
		//}
	}
	return 0;
}

void UMySQLObject::BeginDestroy()
{
	if (bConnectionActive)
	{
		//con->close();

		//con = nullptr;

		bConnectionActive = false;
	}

	if (MySQLThread.joinable()) MySQLThread.join();

	Super::BeginDestroy();
}
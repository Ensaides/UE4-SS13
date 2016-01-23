// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include <memory>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"

#include "MySQLObject.generated.h"



/**
 * 
 */
UCLASS(config = Game)
class SPACESTATIONGAME_API UMySQLObject : public UObject
{
	GENERATED_BODY()
public:
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

	sql::mysql::MySQL_Driver* driver;

	sql::Connection* con;

public:

	void OpenConnection();

	virtual void BeginDestroy() override;

	void SetUpMySQLPlayerData(FString SteamID);

	uint8 GetMySQLPreferredJob(FString SteamID);
};

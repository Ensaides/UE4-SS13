// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager.h"
#include "Job.h"
#include "JobManager.generated.h"

class APawn;

/**
*
*/
USTRUCT(BlueprintType)
struct FJobClass
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Job")
		FString JobName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Job")
		TSubclassOf<UJob> JobClass;

	FJobClass()
	{
		JobName = "Job";
		JobClass = UJob::StaticClass();
	};

	FJobClass(FString inJobName, TSubclassOf<UJob> inJobClass)
	{
		JobName = inJobName;
		JobClass = inJobClass;
	};
};

UCLASS()
class SPACESTATIONGAME_API AJobManager : public AManager
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Called during job initialization, implement this to spawn custom jobs", DisplayName = "Initialize Jobs"), Category = "Jobs")
		void InitializeJobs();

	// Job Classes members
	UFUNCTION(BlueprintCallable, Category = "Jobs")
		static TArray<FJobClass>& GetJobClasses()
		{
			static TArray<FJobClass> JobClasses;

			return JobClasses;
		};

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a job class to the list of jobs", DisplayName = "Add Job Class"), Category = "Jobs")
		static void AddJobClass(FString JobName, TSubclassOf<UJob> NewJob)
		{
			GetJobClasses().Add(FJobClass(JobName, NewJob));
		};

	UFUNCTION(BlueprintCallable, Category = "Jobs")
		static void RemoveJobClass(FString JobName)
		{
			for (int i = 0; i < GetJobClasses().Num(); i++)
			{
				if (GetJobClasses()[i].JobName == JobName)
				{
					GetJobClasses().RemoveAt(i);

					return;
				}
			}
		};

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Set up a job for a player. Called during player spawn", DisplayName = "Setup Job"), Category = "Jobs")
		void SetupJob(APawn* PlayerPawn);
};

// Call this if you are too lazy to put the jobs in the InitializeJobs function
struct DECLARE_JOB
{
	DECLARE_JOB(FString JobName, TSubclassOf<UJob> NewJob)
	{
		AJobManager::AddJobClass(JobName, NewJob);
	};
};
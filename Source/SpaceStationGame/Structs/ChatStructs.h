#pragma once
#include "ChatStructs.generated.h"

#define MESSAGE_DEFAULT_COLOR FLinearColor(0,0,0,1)
#define MESSAGE_NOTIFICATION_COLOR FLinearColor(0.5,0.5,0.5,1)

USTRUCT()
struct FChatMessageStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString PlayerName;

	UPROPERTY()
		FString ChatMsg;

	UPROPERTY()
		FVector Location;

	UPROPERTY()
		uint8 Channel;
};

USTRUCT(BlueprintType)
struct FClientChatMessageStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		FString ChatMsg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		FLinearColor MessageColor;
};
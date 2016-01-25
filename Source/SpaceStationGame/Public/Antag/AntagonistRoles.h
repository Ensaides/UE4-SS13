#pragma once

#include "AntagonistRoles.generated.h"

UENUM(BlueprintType)
enum class EAntagonistRoles : uint8
{

};

USTRUCT(BlueprintType)
struct FPrefferedAntagonistRole
{
	GENERATED_USTRUCT_BODY()

	uint32 bTraitor : 1;
};
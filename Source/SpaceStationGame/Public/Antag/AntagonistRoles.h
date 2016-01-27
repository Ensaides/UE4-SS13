#pragma once

#include "AntagonistRoles.generated.h"

UENUM(BlueprintType)
enum class EAntagonistRoles : uint8
{
	EAntagonistRole_None UMETA(DisplayName = "NONE"),
	EAntagonistRole_Traitor UMETA(DisplayName = "Traitor")
};

USTRUCT(BlueprintType)
struct FPreferredAntagonistRole
{
	GENERATED_USTRUCT_BODY()

	uint32 bTraitor : 1;
};
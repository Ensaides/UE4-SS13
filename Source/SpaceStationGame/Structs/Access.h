#pragma once

#include "Access.generated.h"

USTRUCT(BlueprintType)
struct FAccess
{
	GENERATED_USTRUCT_BODY()

	//Security
	uint32 bBrig : 1;
	uint32 bWeaponPermit : 1;
	uint32 bSecurity : 1;
	uint32 bHoldingCells : 1;
	uint32 bArmory : 1;
	uint32 bForensics : 1;
	uint32 bCourtroom : 1;
	uint32 bHeadOfSecurity : 1;

	//Medical
	uint32 bMedical : 1;


	//General
	uint32 bKitchen : 1;
	uint32 bBar : 1;
	uint32 bHydroponics : 1;
	uint32 bCustodialCloset : 1;
	uint32 bChapelOffice : 1;
	uint32 bCrematorium : 1;
	uint32 bLibrary : 1;
	uint32 bTheatre : 1;
	uint32 bLawOffice : 1;
	uint32 bHonkAccess : 1;
	uint32 bSilentAccess : 1;
	uint32 bMaintenanceAccess : 1;


};
#pragma once

#include "Object.h"
#include "ManagerObject.generated.h"

UCLASS()
class SPACESTATIONGAME_API UManagerObject : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize() {};

};
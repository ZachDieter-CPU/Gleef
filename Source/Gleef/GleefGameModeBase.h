// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GleefGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API AGleefGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGleefGameModeBase();	

	void CheckForHostPlayer(AController* Exiting);

private:
	void HostIsLeaving();
};

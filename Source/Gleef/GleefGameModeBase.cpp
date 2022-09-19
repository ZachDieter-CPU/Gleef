// Copyright Epic Games, Inc. All Rights Reserved.


#include "GleefGameModeBase.h"
#include "BasicCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGleefGameModeBase::AGleefGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_BasicCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void AGleefGameModeBase::CheckForHostPlayer(AController* Exiting)
{
	if (Exiting->HasAuthority())
	{
		HostIsLeaving();
	}
}
void AGleefGameModeBase::HostIsLeaving()
{
	ReturnToMainMenuHost();
}
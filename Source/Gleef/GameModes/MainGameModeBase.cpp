// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "GleefGameInstance.h"
#include "TimerManager.h"

void AMainGameModeBase::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	NumberOfPlayersAlive -= 1;
}

void AMainGameModeBase::RestartPlayer(AController * NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	NumberOfPlayersAlive = GetNumPlayers();
	UE_LOG(LogTemp, Warning, TEXT("%d"), NumberOfPlayersAlive);
}



void AMainGameModeBase::PlayerHasDied()
{
	NumberOfPlayersAlive -= 1;

	if (NumberOfPlayersAlive == 1)
	{
		PlayerHasWon();	
	}
}

void AMainGameModeBase::ResetLevel()
{
	Super::ResetLevel();
}

void AMainGameModeBase::PlayerHasWon()
{
	IsInWinState = true;
	UE_LOG(LogTemp, Warning, TEXT("Player has Won"));

	GetWorldTimerManager().SetTimer(GameStartTimer, this, &AMainGameModeBase::RestartMatchviaServerTravel, 3);
}

void AMainGameModeBase::RestartMatchviaServerTravel()
{
	UWorld* World = GetWorld();
	if(World == nullptr) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/TestMap", true);
}



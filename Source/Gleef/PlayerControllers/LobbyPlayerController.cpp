// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "LobbyGameModeBase.h"
#include "Gleef/MenuSystem/LobbyDetails.h"
#include "Gleef/GameStates/LobbyGameState.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}


void ALobbyPlayerController::SetWidgetAfterCreating(class ULobbyDetails* LobbyDetailsBP)
{
	LobbyDetail = LobbyDetailsBP;
	
	if(LobbyDetail == nullptr) return;
	LobbyDetail->Setup();
}


void ALobbyPlayerController::MulticastUpdateWidgetTimer_Implementation(float Time)
{
	if(LobbyDetail == nullptr) return;
	LobbyDetail->UpdateTimer(Time);
}



void ALobbyPlayerController::MulticastSetWidgetsVisible_Implementation(bool IsVisibleToPlayers)
{
	if(LobbyDetail == nullptr)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Lobby Detail is Null"));
		return;
	}
	if (IsVisibleToPlayers)
	{
		LobbyDetail->SetStartingGameTextBoxToStarting();
	}
	else
	{
		LobbyDetail->SetStartingGameTextBoxToNotStarting();
	}
}

void ALobbyPlayerController::MulticastRemoveWidgets_Implementation()
{
	LobbyDetail->OnLevelRemovedFromWorld(nullptr, nullptr);
}



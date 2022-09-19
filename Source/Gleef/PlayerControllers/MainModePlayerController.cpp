// Fill out your copyright notice in the Description page of Project Settings.


#include "MainModePlayerController.h"
#include "Gleef/MenuSystem/MainGameModeInGameMenu.h"

void AMainModePlayerController::SetWidgetAfterCreating(class UMainGameModeInGameMenu* WBPInGameMenu)
{

	InGameMenu = WBPInGameMenu;
	if(InGameMenu == nullptr){
		UE_LOG(LogTemp, Warning, TEXT("Failed to setup crosshair"));
		return;
	}

	InGameMenu->Setup();
}

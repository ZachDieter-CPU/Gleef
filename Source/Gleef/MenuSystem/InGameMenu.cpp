// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "Gleef/GameModes/MainGameModeBase.h"
bool UInGameMenu::Initialize()
{
	bool Succes = Super::Initialize();
	if (!Succes) return false;
	//Cancel Button pressed
	if (InGameMenuCancelButton == nullptr) return false;
	InGameMenuCancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelInGameMenu);

	//Quit Button pressed
	if (InGameMenuQuitButton == nullptr) return false;
	InGameMenuQuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitButtonPressInGameMenu);

	return true;
}



void UInGameMenu::QuitButtonPressInGameMenu()
{
	if (MenuInterface == nullptr) return;
	APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	if (PlayerController != nullptr && PlayerController->HasAuthority()) {
		
		AGleefGameModeBase* GameModehBase = Cast<AGleefGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameModehBase != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("TEst"));
			GameModehBase->CheckForHostPlayer(PlayerController);
		}
	}
	MenuInterface->LoadMainMenu();
	
}

void UInGameMenu::CancelInGameMenu()
{
	if(!ensure(InGameMenuCancelButton != nullptr))return;
	this->OnLevelRemovedFromWorld(nullptr,nullptr);
}

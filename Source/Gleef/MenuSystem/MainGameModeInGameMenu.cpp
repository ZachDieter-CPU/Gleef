// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeInGameMenu.h"
#include "Components/Image.h"

bool UMainGameModeInGameMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if(Crosshair == nullptr) return false;

	return true;
}

void UMainGameModeInGameMenu::Setup()
{
	this->AddToViewport();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyDetails.h"
#include "Components/TextBlock.h"



bool ULobbyDetails::Initialize()
{
	bool Success = Super::Initialize();

	if(!Success) return false;

	if(StartingGameTextBox == nullptr) return false;
	StartingGameTextBox->SetVisibility(ESlateVisibility::Hidden);
	StartingGameTextBox->SetText(FText::FromString("Starting Game"));

	if(Timer == nullptr) return false;
	Timer->SetVisibility(ESlateVisibility::Hidden);	
	Timer->SetText(FText::FromString(""));

	//auto OwningPlayer = GetOwningPlayer();
		
	if(HowToStart == nullptr) return false;
	//if(OwningPlayer != nullptr && !OwningPlayer->HasAuthority())
	//{
	HowToStart->SetVisibility(ESlateVisibility::Hidden);
	//}
	return true;
}

void ULobbyDetails::UpdateTimer(float Time)
{
	if(Timer == nullptr) return;
	int32 TimeRounded = FMath::RoundToFloat(10 - Time);
	Timer->SetText(FText::FromString(FString::FromInt(TimeRounded)));
}

void ULobbyDetails::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	this->RemoveFromViewport();
}

void ULobbyDetails::Setup()
{
	this->AddToViewport();
}



void ULobbyDetails::Remove()
{
}

void ULobbyDetails::SetStartingGameTextBoxToStarting()
{
	if(StartingGameTextBox == nullptr) return;
	if(Timer == nullptr) return;
	Timer->SetVisibility(ESlateVisibility::Visible);
	StartingGameTextBox->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyDetails::SetStartingGameTextBoxToNotStarting()
{
	if(StartingGameTextBox == nullptr) return;
	if(Timer == nullptr) return;
	Timer->SetVisibility(ESlateVisibility::Hidden);
	StartingGameTextBox->SetVisibility(ESlateVisibility::Hidden);
}



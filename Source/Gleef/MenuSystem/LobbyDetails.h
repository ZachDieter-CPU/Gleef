// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyDetails.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API ULobbyDetails : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual bool Initialize() override;
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StartingGameTextBox;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Timer;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HowToStart;

	void SetStartingGameTextBoxToStarting();

	void SetStartingGameTextBoxToNotStarting();

	void UpdateTimer(float Time);
	void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void Setup();
	void Remove();
};

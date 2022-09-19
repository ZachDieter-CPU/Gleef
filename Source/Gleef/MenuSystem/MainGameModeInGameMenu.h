// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameModeInGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API UMainGameModeInGameMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Crosshair;
	
	void Setup();
};

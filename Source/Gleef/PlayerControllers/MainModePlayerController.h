// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainModePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API AMainModePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetAfterCreating(class UMainGameModeInGameMenu* WBPInGameMenu);


private:
	UPROPERTY()
	class UMainGameModeInGameMenu* InGameMenu;
};

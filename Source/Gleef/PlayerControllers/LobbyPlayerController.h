// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void BeginPlay();

	UFUNCTION(BlueprintCallable)
	void SetWidgetAfterCreating(class ULobbyDetails* LobbyDetailsBP);


	UFUNCTION(NetMulticast, Unreliable)
	void MulticastUpdateWidgetTimer(float Time);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSetWidgetsVisible(bool IsVisibleToPlayers);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRemoveWidgets();
private:
	
	UPROPERTY()
	class ULobbyDetails* LobbyDetail;
		

};

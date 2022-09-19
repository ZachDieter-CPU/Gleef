// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gleef/GleefGameModeBase.h"
#include "LobbyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API ALobbyGameModeBase : public AGleefGameModeBase
{
	GENERATED_BODY()

public:
	
	ALobbyGameModeBase(const FObjectInitializer & ObjectInitializer);

	void PostLogin(APlayerController* NewPlayer);	
	void Logout(AController * Exiting) override;
	void PreLogin(const FString & Options,const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage);
	void HandleStartingNewPlayer(APlayerController * NewPlayer);
	void RestartPlayer(AController * NewPlayer) override;

	void GameStarter();

	void SetIsStarting(bool Setter);
	bool GetIsStarting();
	
	float GetCurrentTimeForTimer();
	void SetCurrentTime(float CurrentTime);
	void StartTime();

	void StopTime();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	


	class ULobbyDetails* LobbyDetail;

private:
	void UpdateEachPlayerWidgetTimer(float Time);
	void StartGame();
	void TellPlayersToSetWidgets(bool IsVisibleToPlayers);
	void TellPlayersToRemoveWidget();
	bool CheckPlayerControllers(class ALobbyPlayerController* LobbyPlayerController);
	uint32 NumberOfPlayers = 0;

	FTimerHandle GameStartTimer;

	TSubclassOf<class UUserWidget> LobbyDetailsClass;

	bool IsStarting;
	bool IsAbleToStart;
	float CurrentStartTimer;	
		
	TArray<APlayerController*> Players;
	
};

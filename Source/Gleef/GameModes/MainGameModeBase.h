// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gleef/GleefGameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API AMainGameModeBase : public AGleefGameModeBase
{
	GENERATED_BODY()


public:
	
	void Logout(AController * Exiting) override;

	void RestartPlayer(AController * NewPlayer) override;
	
	

	UFUNCTION(BlueprintCallable)
	void PlayerHasDied();	
	void ResetLevel() override;
	//Getters
	uint32 GetNumberOfPlayersAlive() { return NumberOfPlayersAlive; };
private:
	FTimerHandle GameStartTimer;
	uint32 NumberOfPlayersAlive;
	void PlayerHasWon();
	void RestartMatchviaServerTravel();
	bool IsInWinState = false;

	
};

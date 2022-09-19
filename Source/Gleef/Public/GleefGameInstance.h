// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gleef/MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GleefGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GLEEF_API UGleefGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:

	UGleefGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init();

	/* Main Menu */
	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	UFUNCTION(Exec)
	void Host(FString ServerName) override;
	UFUNCTION(Exec)
	void Join(uint32 Index) override;
	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	void StartSession();
	virtual void LoadMainMenu() override;
	void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	class UMainMenu* Menu;

	TSubclassOf<class UUserWidget> InGameMenuClass;
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	
	//Call back
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestorySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;

	void CreateSession();

};

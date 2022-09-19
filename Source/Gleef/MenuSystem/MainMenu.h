// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData 
{
	GENERATED_BODY()
	
	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class GLEEF_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);
	void SelectIndex(uint32 Index);	

	void SetServerList(TArray<FServerData> ServerNames);
protected:
	virtual bool Initialize() override;

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	/* Main Menu Code*/
	UPROPERTY(meta = (BindWidget))	
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))	
	class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))	
	class UButton* QuiteGameButton;
	UPROPERTY(meta = (BindWidget))	
	class UButton* CancelJoinMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmJoinMenuButton;
	UPROPERTY(meta = (BindWidget))	
	class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))	
	class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;
	UPROPERTY(meta = (BindWidget))	
	class UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget))	
	class UEditableTextBox* ServerHostName;
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmHostMenuButton;

	UFUNCTION()
	void HostServer();
	UFUNCTION()
	void JoinServer();
	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void OpenHostMenu();
	UFUNCTION()
	void OpenMainMenu();
	UFUNCTION()
	void QuitePressed();
	
	TOptional<uint32> SelectedIndex;
	void UpdateChildren();
};


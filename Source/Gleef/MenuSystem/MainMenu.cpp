// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Blueprints/Menu/MainMenu/WBP_ServerRow"));

	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}


bool UMainMenu::Initialize()
{
	bool Succes = Super::Initialize();
	if(!Succes) return false;	
	
	if(HostButton == nullptr) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	
	if(JoinButton == nullptr) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	//Cancel Buttons
	if (CancelJoinMenuButton == nullptr) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (CancelHostMenuButton == nullptr) return false;
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	//Confirm Buttons
	if (ConfirmJoinMenuButton == nullptr) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (ConfirmHostMenuButton == nullptr) return false;
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (QuiteGameButton == nullptr) return false;
	QuiteGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitePressed);

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr) {
		FString ServerNameEditAbleTextBox = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerNameEditAbleTextBox);
	}
}
void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not set"));
	}
	
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{

	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;
	
	ServerList->ClearChildren();
	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("%d / %d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);
		ServerList->AddChild(Row);
		++i;
	}
	
}

void UMainMenu::OpenJoinMenu()
{
	if(!ensure(JoinMenu != nullptr)) return;
	if(!ensure(MenuSwitcher != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);	
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu()
{
//Needs error checking

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(JoinMenu != nullptr)) return;
	if (!ensure(MenuSwitcher != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitePressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
				
		}
	}
}

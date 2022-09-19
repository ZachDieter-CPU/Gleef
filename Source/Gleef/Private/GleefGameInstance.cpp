// Fill out your copyright notice in the Description page of Project Settings.


#include "GleefGameInstance.h"
#include "Engine/Engine.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Gleef/MenuSystem/MainMenu.h"
#include "Gleef/MenuSystem/MenuWidget.h"
#include "GameFramework/PlayerState.h"
#include "UObject/UnrealNames.h"
//NAME_GameSession

const static FName SESSION_NAME = NAME_GameSession;
//const static FName SESSION_NAME = TEXT("GameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UGleefGameInstance::UGleefGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidget(TEXT("/Game/Blueprints/Menu/MainMenu/WBP_MainMenu"));
	
	if(!ensure(MainMenuWidget.Class != nullptr)) return;
	MenuClass = MainMenuWidget.Class;	

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMainMenuWidget(TEXT("/Game/Blueprints/Menu/InGameMenu/WBP_InGameMenu"));
	if(!ensure(InGameMainMenuWidget.Class != nullptr)) return;
	InGameMenuClass = InGameMainMenuWidget.Class;	


}

void UGleefGameInstance::Init()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();

	if (SubSystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("FOUND subsystem: %s"), *SubSystem->GetSubsystemName().ToString());

		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGleefGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGleefGameInstance::OnDestorySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGleefGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGleefGameInstance::OnJoinSessionComplete);
			
		}
	}
}

void UGleefGameInstance::LoadMenu()
{
	if(!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if(!ensure(Menu != nullptr)) return;

	Menu->Setup();	
	Menu->SetMenuInterface(this);
}
void UGleefGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if(SessionInterface.IsValid()){
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);	
		}
		else
		{
			CreateSession();	
		}
	}
}
void UGleefGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	UEngine* Engine = GetEngine();

		
	if (Engine) {
		Engine->AddOnScreenDebugMessage(-1, 2, FColor::Black, TEXT("Hosting"));
	}

	UWorld* World = GetWorld();

	if (World) {
		World->ServerTravel("/Game/Maps/Lobby?listen");
	}
}

void UGleefGameInstance::OnDestorySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UGleefGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		UE_LOG(LogTemp, Warning, TEXT("STARTING TO FIND SESSION"));
		SessionSearch->MaxSearchResults = 100; 
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UGleefGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ending find session"));
		TArray<FServerData> ServerNames;
		
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("FOUND session, %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;
			Data.Name = SearchResult.GetSessionIdStr();
			FString ServerNameForGetQuery;
			if(SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerNameForGetQuery))
			{
				Data.Name = ServerNameForGetQuery;
			}
			else
			{
				Data.Name = "Could not find Name";
			}
			ServerNames.Add(Data);				
		}
		Menu->SetServerList(ServerNames);
	}
}

void UGleefGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("COULD NOT GET CONNECT STRING"));
		return;
	}

	UEngine* Engine = GetEngine();
	if(!ensure(Engine != nullptr)) return;	

	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;	

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UGleefGameInstance::CreateSession()
{
	if(SessionInterface.IsValid()){
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}else
		{
			SessionSettings.bIsLANMatch = false;
		}

		SessionSettings.NumPublicConnections = 8;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UGleefGameInstance::Join(uint32 Index)
{

	if(!SessionInterface.IsValid()) return;	
	if(!SessionSearch.IsValid()) return;
	//if (Menu != nullptr) {
	//	Menu->SetServerList({"Test1","Test2"});
	//}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);	
	
}

void UGleefGameInstance::InGameLoadMenu()
{

	if(!ensure(InGameMenuClass != nullptr)) return;

  	UMenuWidget* InGameMenuTemp = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	InGameMenuTemp->Setup();
	InGameMenuTemp->SetMenuInterface(this);
	

}

void UGleefGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UGleefGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/Blueprints/Menu/MainMenu/MainMenu", ETravelType::TRAVEL_Absolute);

}




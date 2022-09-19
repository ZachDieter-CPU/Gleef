// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameModeBase.h"
#include "TimerManager.h"
#include "GleefGameInstance.h"
#include "Gleef/GameStates/LobbyGameState.h"
#include "Gleef/PlayerControllers/LobbyPlayerController.h"

ALobbyGameModeBase::ALobbyGameModeBase(const FObjectInitializer & ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	IsStarting = false;
	IsAbleToStart = true;
}

void ALobbyGameModeBase::Tick(float DeltaTime)
{
	if(HasAuthority())
	{
		if (GetIsStarting())
		{
			float Time = GetWorldTimerManager().GetTimerElapsed(GameStartTimer);
			UpdateEachPlayerWidgetTimer(Time);
		}
	}
	int count = 0;
	for (auto& PlayerC : Players)
	{
		count+=1;
	}
}
void ALobbyGameModeBase::UpdateEachPlayerWidgetTimer(float Time)
{
	for (auto& PlayerC : Players)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerC);
		if (LobbyPlayerController != nullptr && CheckPlayerControllers(LobbyPlayerController))
		{
			LobbyPlayerController->MulticastUpdateWidgetTimer(Time);
		}
	}
}

void ALobbyGameModeBase::PreLogin(const FString & Options, const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	UINT32 NumOfPlayerCheck = NumberOfPlayers + 1;

	if (GetWorldTimerManager().IsTimerActive(GameStartTimer))
	{
		ErrorMessage = "Game has Started";
		return;
	}

	if (NumOfPlayerCheck > 8)
	{
		ErrorMessage = "Not Enough Slots";
		return;
	}
}

void ALobbyGameModeBase::RestartPlayer(AController * NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Restart Player"));
	IsAbleToStart = false;
}

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	if (NumberOfPlayers == 8)
	{
		StartTime();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	Players.Add(NewPlayer);
	IsAbleToStart = true;
}

void ALobbyGameModeBase::HandleStartingNewPlayer(APlayerController * NewPlayer)
{
	Super::HandleStartingNewPlayer(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Handling player start"));
}




void ALobbyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UE_LOG(LogTemp, Warning, TEXT("Loging out"));
	StopTime();
	--NumberOfPlayers;
	auto PlayerController = Cast<APlayerController>(Exiting);

	if(PlayerController){
		Players.Remove(PlayerController);
	}
}

void ALobbyGameModeBase::GameStarter()
{
	//ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	
	if (!GetIsStarting())
	{
		//UE_LOG(LogTemp, Warning, TEXT("IsAbleToStart : %s"), IsAbleToStart? TEXT("True") : TEXT("False"));
		SetIsStarting(true);
		StartTime();
	}
	else
	{
		SetIsStarting(false);
		StopTime();	
	}
}





void ALobbyGameModeBase::StartGame()
{
	SetIsStarting(false);

	auto GameInstance = Cast<UGleefGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;
	
	TellPlayersToRemoveWidget();

	World->ServerTravel("/Game/Maps/TestMap?listen", true);
}

void ALobbyGameModeBase::TellPlayersToSetWidgets(bool IsVisibleToPlayers)
{
	for (auto& PlayerC : Players)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerC); 
		if (LobbyPlayerController != nullptr && CheckPlayerControllers(LobbyPlayerController))
		{
			LobbyPlayerController->MulticastSetWidgetsVisible(IsVisibleToPlayers);
		}
	}
}



void ALobbyGameModeBase::TellPlayersToRemoveWidget()
{
	for (auto& PlayerC : Players)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerC);
		if (LobbyPlayerController != nullptr && CheckPlayerControllers(LobbyPlayerController))
		{
			LobbyPlayerController->MulticastRemoveWidgets();
		}
	}
}

bool ALobbyGameModeBase::CheckPlayerControllers(ALobbyPlayerController* LobbyPlayerController)
{
	if (!LobbyPlayerController->IsActorInitialized()){
		UE_LOG(LogTemp, Warning, TEXT("Actor is not Init"));	
		return false;
	}
	if(LobbyPlayerController->IsUnreachable()){ 
		UE_LOG(LogTemp, Warning, TEXT("Actor is not reachable"));	
		return false;
	}
	return true;
}

void ALobbyGameModeBase::StartTime()
{
	if(HasAuthority())
	{
		TellPlayersToSetWidgets(true);	
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameModeBase::StartGame, 10);
	}
}

void ALobbyGameModeBase::StopTime()
{
	if(HasAuthority())
	{
		TellPlayersToSetWidgets(false);	
		GetWorldTimerManager().ClearTimer(GameStartTimer);
	}
}

void ALobbyGameModeBase::SetIsStarting(bool Setter)
{
	IsStarting = Setter;
}

bool ALobbyGameModeBase::GetIsStarting()
{
	return IsStarting;
}

float ALobbyGameModeBase::GetCurrentTimeForTimer()
{
	return CurrentStartTimer;
}

void ALobbyGameModeBase::SetCurrentTime(float CurrentTime)
{
	CurrentStartTimer = CurrentTime;
}


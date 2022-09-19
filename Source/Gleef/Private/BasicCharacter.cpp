// Fill out your copyright notice in the Description page of Project Settings.



#include "BasicCharacter.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProjectTile.h"
#include "Grid.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GleefGameInstance.h"
#include "LobbyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/CharacterMovementComponent.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	/*Ability Options*/
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");


	/*Line Trace Options*/
	TraceDistance = 250;
	
	/*Collision options*/
	GetCapsuleComponent()->InitCapsuleSize(10.f, 96.0f);
	//BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	

	RateOfFire = 600;
}



// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
		
	TimeBetweenShots = 60/RateOfFire;
}

void ABasicCharacter::SimpleAction()
{

}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//Action bindings
	PlayerInputComponent->BindAction("SimpleAction", IE_Pressed, this, &ABasicCharacter::SimpleAction);
	PlayerInputComponent->BindAction("RangedBasicAttack", IE_Pressed, this, &ABasicCharacter::SimpleAction);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABasicCharacter::StopJump);
	PlayerInputComponent->BindAction("PauseGame",IE_Pressed, this, &ABasicCharacter::PauseGame);
	PlayerInputComponent->BindAction("StartandStopGame",IE_Pressed, this, &ABasicCharacter::StartandStopGame);
	//Keyboard Movement
	PlayerInputComponent->BindAxis("Forward", this, &ABasicCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ABasicCharacter::MoveRight);

	//Mouse Movement
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look", this, &APawn::AddControllerPitchInput);

}
UAbilitySystemComponent* ABasicCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ABasicCharacter::AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
	if (AbilitySystemComp)
	{
		if (HasAuthority() && AbilityToAquire)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityToAquire, 1, 0));
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

void ABasicCharacter::AttackWithLineTrace()
{

	//Set up for the LineTrace
	FVector pLocation, Start, End;
	FRotator pRotation;
	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	GetController()->GetPlayerViewPoint(pLocation, pRotation);

	Start = pLocation;
	End = Start + (pRotation.Vector() * TraceDistance);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	HitActor = Hit;

	//Delete when done
	//DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, 2.0);

	//Looking For Grid and calling Remove Block on that block
	if (bHit) {
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Red, false, 2.0f);
		AGrid* GridOfBlocks = Cast<AGrid>(Hit.GetActor());

		if (GridOfBlocks) {
			GridOfBlocks->RemoveBlockFromGrid(Hit.Item);
			UE_LOG(LogTemp, Warning, TEXT("Index %d"), Hit.Item);
		}
	}

}




void ABasicCharacter::AttackWithProjectTile()
{
	if (ProjectileClass == nullptr) return;
	//if(!HasAuthority()) return;

	// Get the camera transform.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Transform MuzzleOffset from camera space to world space.
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
	FRotator MuzzleRotation = CameraRotation;
	// Skew the aim to be slightly upwards.
	MuzzleRotation.Pitch += 10.0f;
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		// Spawn the projectile at the muzzle.
		AProjectTile* Projectile = World->SpawnActor<AProjectTile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			FVector LaunchDirection = MuzzleRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
		}
	}
	LastFireTime = GetWorld()->TimeSeconds;
}


void ABasicCharacter::MoveForward(float val)
{
	if (val != 0 && (Controller)) {
		AddMovementInput(GetActorForwardVector(), val);
	}
}

void ABasicCharacter::MoveRight(float val)
{
	if (val != 0 && (Controller)) {
		AddMovementInput(GetActorRightVector(), val);
	}
}

void ABasicCharacter::PauseGame()
{

	UGleefGameInstance* GameInstance = Cast<UGleefGameInstance>(GetGameInstance());
	
	if(GameInstance == nullptr) return;

	GameInstance->InGameLoadMenu();
}

void ABasicCharacter::StartandStopGame()
{

	if(!HasAuthority()) return;
	//auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		//if(PlayerController == nullptr) return;
	//PlayerController->HostStart();

	ALobbyGameModeBase* LobbyGameMode = Cast<ALobbyGameModeBase>(GetWorld()->GetAuthGameMode());
	if(LobbyGameMode == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Starting Timer"));	
	LobbyGameMode->GameStarter();

}


void ABasicCharacter::StartJump()
{
	bPressedJump = true;
}

void ABasicCharacter::StopJump()
{
	bPressedJump = false;
}


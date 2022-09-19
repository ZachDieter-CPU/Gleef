// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "BasicCharacter.generated.h"

class UCameraComponent;
class UBoxComponent;


UCLASS()
class GLEEF_API ABasicCharacter : public ACharacter, public IAbilitySystemInterface 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/*Inputs*/

	//Actions
	UFUNCTION(BlueprintCallable, Category = "BasicActions")
	void SimpleAction();
	UFUNCTION(BlueprintCallable, Category = "BasicActions")
	void PauseGame();
	UFUNCTION(BlueprintCallable, Category = "BasicActions")
	void StartandStopGame();


	

	//Movement
	void StartJump();
	void StopJump();
	void MoveForward(float val);
	void MoveRight(float val);
	/*End of inputs*/




	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float TraceDistance;


	FTimerHandle TimerHandle_TimeBetweenProjectiles;

	float LastFireTime;
	

	//RMP - Bullets per minute fired
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	//Derived from RateOfFire
	float TimeBetweenShots;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAbilitySystemComponent* AbilitySystemComp;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Characterbase")
	void AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

	// Gun muzzle's offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectTile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category = "LineTrace")
	void AttackWithLineTrace();
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void AttackWithProjectTile();

private:
	AActor* FocusedActor;
	FHitResult HitActor;

};

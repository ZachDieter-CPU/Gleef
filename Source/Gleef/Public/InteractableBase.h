// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractableBase.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class GLEEF_API AInteractableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	
	UPROPERTY( VisibleDefaultsOnly , BlueprintReadOnly, Category = "HISM")
		UHierarchicalInstancedStaticMeshComponent* Stage;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* Caller,  FHitResult HitActor);
	virtual void OnInteract_Implementation(AActor* Caller,  FHitResult HitActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartFocus(FHitResult HitActor);
	virtual void StartFocus_Implementation(FHitResult HitActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndFocus(FHitResult HitActor);
	virtual void EndFocus_Implementation(FHitResult HitActor);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
	Stage = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMeshComponent"));

}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
		
}


void AInteractableBase::OnInteract_Implementation(AActor * Caller, FHitResult HitActor)
{
	//FName test = HitActor->GetFName();
	//UE_LOG(LogTemp, Warning, TEXT("%s"),*test.ToString());
}

void AInteractableBase::StartFocus_Implementation(FHitResult HitActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Start Focus"));
}

void AInteractableBase::EndFocus_Implementation(FHitResult HitActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("end Focus"));
	
}


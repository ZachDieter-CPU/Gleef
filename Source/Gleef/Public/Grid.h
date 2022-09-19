// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class UInstancedStaticMeshComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UParticleSystem;

UCLASS()
class GLEEF_API AGrid : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AGrid();
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	UFUNCTION(BlueprintCallable)
	void setHism(UHierarchicalInstancedStaticMeshComponent* HISM);

	UHierarchicalInstancedStaticMeshComponent* BlockHism;

	UFUNCTION(BlueprintImplementableEvent, Category = "Grid")
	void PlayBlockBreakEvent(int IndexOfBlock, UHierarchicalInstancedStaticMeshComponent* BLockHism);
public:	

	UFUNCTION(BlueprintCallable, Category = "LevelGrid")
	void RemoveBlockFromGrid(int32 IndexOfBlock);

	UFUNCTION(BlueprintCallable)
	UHierarchicalInstancedStaticMeshComponent* GetGrid();
};

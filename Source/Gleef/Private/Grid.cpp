// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GleefGameState.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AGrid::AGrid()
{
}


// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrid::setHism(UHierarchicalInstancedStaticMeshComponent* HISM)
{
	BlockHism = HISM;
}


void AGrid::RemoveBlockFromGrid(int32 IndexOfBlock)
{
	//Plays Cloud effect	
	this->PlayBlockBreakEvent(IndexOfBlock, BlockHism);	


	//This is to tell all the other clients to remove the same block
	AGleefGameState* GleefGameState = Cast<AGleefGameState>(GetWorld()->GetGameState());
	if (GleefGameState) {
		GleefGameState->MulticastRemoveBlockFromGrid(BlockHism, IndexOfBlock);
	}
}

UHierarchicalInstancedStaticMeshComponent* AGrid::GetGrid()
{
	return BlockHism;
}


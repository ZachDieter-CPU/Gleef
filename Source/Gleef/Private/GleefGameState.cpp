// Fill out your copyright notice in the Description page of Project Settings.


#include "GleefGameState.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"




/*Tell Other players to remove the block as well*/
void AGleefGameState::MulticastRemoveBlockFromGrid_Implementation(UHierarchicalInstancedStaticMeshComponent* Grid, int32 IndexOfBlock)
{
	if(Grid == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Block Should be Removed"));
	Grid->RemoveInstance(IndexOfBlock);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GleefGameState.generated.h"


class UHierarchicalInstancedStaticMeshComponent;
/**
 * 
 */
UCLASS()
class GLEEF_API AGleefGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRemoveBlockFromGrid(UHierarchicalInstancedStaticMeshComponent* Grid, int32 IndexOfBlock);


};

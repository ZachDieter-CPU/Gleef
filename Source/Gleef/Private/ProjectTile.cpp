// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectTile.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Grid.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectTile::AProjectTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	// Set the sphere's collision radius.
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectTile::OnHit);
	// Set the root component to be the collision component.
	RootComponent = CollisionComponent;

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	// Die after 3 seconds.
	InitialLifeSpan = 3.0f;
	
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AProjectTile::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AProjectTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectTile::FireInDirection(const FVector& ShootDirection)
{
	if(ProjectileMovementComponent == nullptr) return;
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}



void AProjectTile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority()) {
		return;
	}

	if (OtherActor != this)
	{
		AGrid *GridBlocks = Cast<AGrid>(OtherActor);
		if (GridBlocks) {
			UE_LOG(LogTemp, Warning, TEXT("CAST SUCCESFUL"));
			GridBlocks->RemoveBlockFromGrid(Hit.Item);
		}
	}
	Destroy();
}


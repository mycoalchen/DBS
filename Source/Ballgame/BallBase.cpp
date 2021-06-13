// Fill out your copyright notice in the Description page of Project Settings.

#include "BallBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABallBase::ABallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	StaticMesh->SetMobility(EComponentMobility::Movable);
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(FName("PMC"));
	PMC->ProjectileGravityScale = 1;
}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();
	
}

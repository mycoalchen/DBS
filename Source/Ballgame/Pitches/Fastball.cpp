// Fill out your copyright notice in the Description page of Project Settings.


#include "Fastball.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFastball::AFastball()
{
	SpinRotator = FRotator(1, 0, 0);
	NumBlurMeshes = 10;
	BlurMeshAngle = -5;
	BlurMeshMaxOpacity = 0.8;
}

void AFastball::BeginPlay()
{
	Super::BeginPlay();
	// PMC->ProjectileGravityScale = 0.2;
	StartLocation = GetActorLocation();
	PrimaryActorTick.bCanEverTick = true;
	PrevX = GetActorLocation().X;
}

void AFastball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PrevX = GetActorLocation().X;
}

void AFastball::PhysicsTick()
{
	// Deceleration since last frame
	const float Deceleration = (PrevX - GetActorLocation().X) * 0.20952381622;
	PMC->Velocity += FVector(Deceleration, 0, 0);
}

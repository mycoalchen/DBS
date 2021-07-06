// Fill out your copyright notice in the Description page of Project Settings.


#include "Fastball.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFastball::AFastball()
{
	PMC->ProjectileGravityScale = 0.2;
	SpinRotator = FRotator(1, 0, 0);
}

void AFastball::BeginPlay()
{
	Super::BeginPlay();
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
	const float Deceleration = (PrevX - GetActorLocation().X) * 0.4687; // 0.4687
	PMC->Velocity += FVector(Deceleration, 0, 0);
}

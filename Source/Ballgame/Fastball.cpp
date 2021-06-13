// Fill out your copyright notice in the Description page of Project Settings.


#include "Fastball.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFastball::AFastball()
{
	PMC->ProjectileGravityScale = 0.7;
}

void AFastball::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	PrimaryActorTick.bCanEverTick = true;
	SetActorRotation(FRotator(0, 90, 0));
}

void AFastball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(FRotator(1, 0, 0) * SpinRateRPM * 6 * DeltaTime);
	PrevX = GetActorLocation().X;
}

void AFastball::PhysicsTick()
{
	const FVector2D HorizontalSpeed = GetVelocity().X;
	// Deceleration since last frame
	// const float Deceleration = (PrevX - GetActorLocation().X) * 3.28084 / 
}

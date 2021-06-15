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
	PrevX = GetActorLocation().X;
}

void AFastball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(FRotator(1, 0, 0) * SpinRateRPM * 6 * DeltaTime);
	PhysicsTick();
	PrevX = GetActorLocation().X;
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::SanitizeFloat(-PMC->Velocity.X));
}

void AFastball::PhysicsTick()
{
	const FVector2D HorizontalSpeed = GetVelocity().X;
	// Deceleration since last frame
	const float Deceleration = (PrevX - GetActorLocation().X) * 0.4687; // 0.4687
	PMC->Velocity += FVector(Deceleration, 0, 0);
}

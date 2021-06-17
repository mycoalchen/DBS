// Fill out your copyright notice in the Description page of Project Settings.


#include "Pitcher.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Fastball.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APitcher::APitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	SetRootComponent(StaticMesh);
	ReleasePoint = CreateDefaultSubobject<USceneComponent>(FName("ReleasePoint"));
	PitchTarget = CreateDefaultSubobject<USceneComponent>(FName("PitchTarget"));
	PitchTarget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APitcher::BeginPlay()
{
	Super::BeginPlay();
	if (FastballClass == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("No fastball class in Pitcher"));
}

// Called every frame
void APitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceLastThrow += DeltaTime;
	/*if (TimeSinceLastThrow > TimeBetweenThrows)
	{
		ThrowFastball();
		TimeSinceLastThrow = 0;
	}*/
	DrawDebugSphere(GetWorld(), ReleasePoint->GetComponentLocation(), 6,
		10, FColor::Red, false, 0.025, 0, 0);
}

void APitcher::ThrowFastball()
{
	if (!Fastball) return;
	// Spawn the ball at the correct location
	GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::White, TEXT("Threw ball"));
	const FVector SpawnLocation = ReleasePoint->GetComponentLocation();
	const FRotator SpawnRotation = FRotator(0, 90, 0);
	Fastball->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Here"));
	
	Fastball->PMC->InitialSpeed = FastballSpeedMPH * 44.7;
	Fastball->PMC->MaxSpeed = 0;
	Fastball->PMC->Velocity = FVector(-FastballSpeedMPH * 44.7, 0, 0);
	// Ball->PMC->SetVelocityInLocalSpace(FVector(0, FastballSpeedMPH * 44.7, 0));
	Fastball->SetLifeSpan(TimeBetweenThrows);
	Fastball->SpinRateRPM = FastballSpinRateRPM;
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Pitcher.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Fastball.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerCharacter.h"
#include "CinematicCamera/Public/CineCameraComponent.h"

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

AFastball* APitcher::ThrowFastball(float MPH, float SpinRate)
{
	// Spawn the ball at the correct location
	const FActorSpawnParameters SpawnParams;
	const FVector SpawnLocation = ReleasePoint->GetComponentLocation();
	const FRotator SpawnRotation = FastballStartRotator;
	AFastball* ball = GetWorld()->SpawnActor<AFastball>(FastballClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	ball->PMC->InitialSpeed = MPH * 44.7;
	ball->PMC->MaxSpeed = 0;
	ball->PMC->Velocity = FVector(-MPH * 44.7, 0, 0);
	ball->SetLifeSpan(TimeBetweenThrows);
	ball->SpinRateRPM = SpinRate;

	APlayerCharacter* Player = StaticCast<APlayerCharacter*>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player->BatterCamera)
		Player->BatterCamera->FocusSettings.TrackingFocusSettings.ActorToTrack = ball;

	TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(static_cast<int32>(MPH)));
	args.Add(FStringFormatArg(static_cast<int32>(SpinRate)));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Format(TEXT("Fastball- {0} mph, {1} rpm"), args));

	return ball;
}



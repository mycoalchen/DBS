// Fill out your copyright notice in the Description page of Project Settings.


#include "Pitcher.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Fastball.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerCharacter.h"
#include "MyGSB.h"
#include "CinematicCamera/Public/CineCameraComponent.h"

// Sets default values
APitcher::APitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BodyMesh"));
	SetRootComponent(Body_Mesh);
	ReleasePoint = CreateDefaultSubobject<USceneComponent>(FName("ReleasePoint"));
}

// Called when the game starts or when spawned
void APitcher::BeginPlay()
{
	Super::BeginPlay();
	if (FastballClass == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("No fastball class in Pitcher"));
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ReleasePoint->AttachToComponent(Body_Mesh, Rules, FName("rightHandThrow"));
	AMyGSB* GameState = Cast<AMyGSB>(GetWorld()->GetGameState());
	if (GameState) GameState->Pitcher = this;
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
}

void APitcher::ThrowFastball(float MPH, float SpinRate)
{
	// Don't throw a pitch if there's already an active ball
	AMyGSB* GameState = Cast<AMyGSB>(GetWorld()->GetGameState());
	if (GameState && GameState->PlayerCharacter->ActiveBall || !GameState) return;
	
	// Spawn the ball at the correct location
	const FActorSpawnParameters SpawnParams;
	const FVector SpawnLocation = ReleasePoint->GetComponentLocation();
	const FRotator SpawnRotation = FastballStartRotator;
	AFastball* ball = GetWorld()->SpawnActor<AFastball>(FastballClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	ball->PMC->InitialSpeed = MPH * 44.7;
	ball->PMC->MaxSpeed = 0;
	ball->PMC->Velocity = FVector(-MPH * 44.7, FMath::FRandRange(-70, 70), FMath::FRandRange(-125, 0));
	ball->SetLifeSpan(TimeBetweenThrows);
	ball->SpinRateRPM = SpinRate;

	APlayerCharacter* Player = StaticCast<APlayerCharacter*>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player->BatterCamera)
		Player->BatterCamera->FocusSettings.TrackingFocusSettings.ActorToTrack = ball;

	TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(static_cast<int32>(MPH)));
	args.Add(FStringFormatArg(static_cast<int32>(SpinRate)));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Format(TEXT("Fastball- {0} mph, {1} rpm"), args));

	GameState->PlayerCharacter->ActiveBall = ball;
	
	PlayPitchAnimation();
}



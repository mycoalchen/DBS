// Fill out your copyright notice in the Description page of Project Settings.


#include "Pitcher.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Pitches/Fastball.h"
#include "Pitches/Curveball.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Controllers/PlayerCharacter.h"
#include "Controllers/PrecisionController.h"
#include "UI/PrecisionTrainingSidebar.h"
#include "Components/CapsuleComponent.h"
#include "Framework/MyGSB.h"
#include "CinematicCamera/Public/CineCameraComponent.h"

// Sets default values
APitcher::APitcher()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule"));
	SetRootComponent(Capsule);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BodyMesh"));
	Body_Mesh->SetupAttachment(Capsule);
	ReleasePoint = CreateDefaultSubobject<USceneComponent>(FName("ReleasePoint"));
	ReleasePoint->SetupAttachment(Body_Mesh);
}

// Called when the game starts or when spawned
void APitcher::BeginPlay()
{
	Super::BeginPlay();
	if (FastballClass == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("No fastball class in Pitcher"));
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
	FTimerHandle WaitHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ThrowFastball2"), MPH, SpinRate);
	PlayPitchAnimation();
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, TimerDelegate, ReleaseDelay, false);
	
}

void APitcher::ThrowFastball2(float MPH, float SpinRate)
{
	// Don't throw a pitch if there's already an active ball
	AMyGSB* GameState = Cast<AMyGSB>(GetWorld()->GetGameState());
	if (GameState && GameState->PlayerCharacter->ActiveBall || !GameState) return;

	// Spawn the ball at the correct location
	const FActorSpawnParameters SpawnParams;
	const FVector SpawnLocation = ReleasePoint->GetComponentLocation();
	const FRotator SpawnRotation = FastballStartRotator;
	AFastball* ball = GetWorld()->SpawnActor<AFastball>(FastballClass, SpawnLocation, SpawnRotation, SpawnParams);
	ball->RegisterAllComponents();
	// Always crashes on this line for some reason
	if (ball->PMC)
	{
	ball->PMC->InitialSpeed = MPH * 44.7;
	ball->PMC->MaxSpeed = 0;
	ball->PMC->Velocity = FVector(-MPH * 44.7, FMath::FRandRange(-40, 40), FMath::FRandRange(-125, -35));
	ball->SetLifeSpan(TimeBetweenThrows);
	ball->SpinRateRPM = SpinRate;
	ball->SpeedMPH = MPH;
	ball->PitchType = 1;
	}

	APlayerCharacter* Player = StaticCast<APlayerCharacter*>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player->BatterCamera)
		Player->BatterCamera->FocusSettings.TrackingFocusSettings.ActorToTrack = ball;

	TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(static_cast<int32>(MPH)));
	args.Add(FStringFormatArg(static_cast<int32>(SpinRate)));
	
	APrecisionController* PC = Cast<APrecisionController>(GameState->PlayerCharacter);
	if (PC)
	{
		PC->CanSwing = true;
		// Balances difficulty
		PC->SwingDuration = -0.0016 * (MPH - 85) + PC->BaseSwingDuration;
	}
	GameState->PlayerCharacter->ActiveBall = ball;
}

void APitcher::ThrowCurveball(float MPH, float SpinRate)
{
	FTimerHandle WaitHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ThrowCurveball2"), MPH, SpinRate);
	PlayPitchAnimation();
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, TimerDelegate, ReleaseDelay, false);

}

void APitcher::ThrowCurveball2(float MPH, float SpinRate)
{
	// Don't throw a pitch if there's already an active ball
	AMyGSB* GameState = Cast<AMyGSB>(GetWorld()->GetGameState());
	if (GameState && GameState->PlayerCharacter->ActiveBall || !GameState) return;

	// Spawn the ball at the correct location
	const FActorSpawnParameters SpawnParams;
	ACurveball* ball = GetWorld()->SpawnActor<ACurveball>(CurveballClass, ReleasePoint->GetComponentLocation(), CurveballStartRotator, SpawnParams);
	ball->RegisterAllComponents();
	ball->PMC->InitialSpeed = MPH * 44.7;
	ball->PMC->MaxSpeed = 0;
	ball->PMC->Velocity = FVector(-MPH * 44.7, FMath::FRandRange(-40, 40), FMath::FRandRange(-125, -35));
	ball->SetLifeSpan(TimeBetweenThrows);
	ball->SpinRateRPM;
	ball->SpeedMPH = MPH;
	ball->PitchType = 2;

	APlayerCharacter* Player = StaticCast<APlayerCharacter*>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player->BatterCamera)
		Player->BatterCamera->FocusSettings.TrackingFocusSettings.ActorToTrack = ball;

	TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(static_cast<int32>(MPH)));
	args.Add(FStringFormatArg(static_cast<int32>(SpinRate)));
	
	APrecisionController* PC = Cast<APrecisionController>(GameState->PlayerCharacter);
	if (PC)
	{
		PC->CanSwing = true;
		// Balances difficulty
		PC->SwingDuration = -0.0016 * (MPH - 85) + PC->BaseSwingDuration;
	}
	GameState->PlayerCharacter->ActiveBall = ball;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "PrecisionController.h"
#include "BallBase.h"
#include "SwingReticle.h"
#include "MyGSB.h"
#include "Blueprint/UserWidget.h"
#include "Components/ContentWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "CineCameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/Image.h"

APrecisionController::APrecisionController()
{
	SwingSphere = CreateDefaultSubobject<USphereComponent>(FName("SwingSphere"));
	SwingSphere->SetSphereRadius(SwingSphereRadius);
	SwingSphere->SetActive(false);
}

void APrecisionController::BeginPlay()
{
	Super::BeginPlay();
	CreateUI();
	SwingSphere->OnComponentBeginOverlap.AddDynamic(this, &APrecisionController::OnSwingSphereOverlapped);

}

void APrecisionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void APrecisionController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MouseX"), this, &APrecisionController::MouseX);
	PlayerInputComponent->BindAxis(FName("MouseY"), this, &APrecisionController::MouseY);
	PlayerInputComponent->BindAction(FName("LMB"), IE_Pressed, this, &APrecisionController::LeftClick);
}


void APrecisionController::CreateUI()
{
	if (ReticleClass)
	{
		Reticle = CreateWidget<USwingReticle>(GetWorld(), ReticleClass, FName(TEXT("Reticle")));
		Reticle->AddToViewport();
	}
}

void APrecisionController::MouseX(float Value)
{
	if (Reticle && Reticle->ReticleImage)
	{
		if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Reticle->ReticleImage->Slot))
			ImageSlot->SetPosition(ImageSlot->GetPosition() + FVector2D(ReticleSensitivity * Value, 0));
	}
}

void APrecisionController::MouseY(float Value)
{
	if (Reticle && Reticle->ReticleImage)
	{
		if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Reticle->ReticleImage->Slot))
			ImageSlot->SetPosition(ImageSlot->GetPosition() + FVector2D(0, -ReticleSensitivity * Value));
	}
}

void APrecisionController::LeftClick()
{
	FVector2D ReticlePosition, ViewportSize;
	FVector WorldPosition, WorldDirection;
	if (Reticle && Reticle->ReticleImage)
	{
		if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Reticle->ReticleImage->Slot))
			ReticlePosition = ImageSlot->GetPosition();
	}
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	
	if (PC->DeprojectScreenPositionToWorld(ReticlePosition.X + ViewportSize.X * 0.5, ReticlePosition.Y + ViewportSize.Y * 0.5, WorldPosition, WorldDirection))
	{
		float scaleFactor = SwingSphereXDistance / (WorldPosition.X - BatterCamera->GetComponentLocation().X);
		FVector SwingLocation = FVector(BatterCamera->GetComponentLocation() + FVector(SwingSphereXDistance,
			scaleFactor * (WorldPosition.Y - BatterCamera->GetComponentLocation().Y),
			scaleFactor * (WorldPosition.Z - BatterCamera->GetComponentLocation().Z)));
		DrawDebugSphere(GetWorld(), SwingLocation, SwingSphereRadius, 20, FColor::White, false, SwingSphereDuration);
		SwingSphere->SetActive(true);
		SwingSphere->SetWorldLocation(SwingLocation);
		GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &APrecisionController::OnSwingFinished, SwingSphereDuration);
	}
	
}

void APrecisionController::OnBallWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{
		ActiveBall = nullptr;
		GetWorld()->DestroyActor(Ball);
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Called"));
	}
}

void APrecisionController::OnSwingSphereOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Target hit!"));
	}
}


void APrecisionController::OnSwingFinished()
{
	SwingSphere->SetActive(false);
	GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
}

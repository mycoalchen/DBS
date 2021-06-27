// Fill out your copyright notice in the Description page of Project Settings.


#include "PrecisionController.h"
#include "SwingReticle.h"
#include "Blueprint/UserWidget.h"
#include "Components/ContentWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"
#include "Components/SceneComponent.h"
#include "CineCameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/Image.h"

APrecisionController::APrecisionController()
{
	SwingStartPoint = CreateDefaultSubobject<USceneComponent>(FName("SwingStartPoint"));
}

void APrecisionController::BeginPlay()
{
	Super::BeginPlay();
	CreateUI();
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
	if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Reticle->ReticleImage->Slot))
	{
		ImageSlot->SetPosition(ImageSlot->GetPosition() + FVector2D(ReticleSensitivity * Value, 0));
	}
}

void APrecisionController::MouseY(float Value)
{
	if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Reticle->ReticleImage->Slot))
	{
		ImageSlot->SetPosition(ImageSlot->GetPosition() + FVector2D(0, -ReticleSensitivity * Value));
	}
}

void APrecisionController::LeftClick()
{
	FVector2D ReticlePosition, ViewportSize;
	FVector WorldPosition, WorldDirection;
	if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Reticle->ReticleImage->Slot))
		ReticlePosition = ImageSlot->GetPosition();
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("BallTrace")));
	FCollisionObjectQueryParams ObjectParams = FCollisionObjectQueryParams();
	FHitResult HitResult(ForceInit);
	
	if (PC->DeprojectScreenPositionToWorld(ReticlePosition.X + ViewportSize.X * 0.5, ReticlePosition.Y + ViewportSize.Y * 0.5, WorldPosition, WorldDirection))
	{
		DrawDebugSphere(GetWorld(), WorldPosition, 0.2, 20, FColor::White, false, 1);
		// DrawDebugLine(GetWorld(), BatterCamera->GetComponentLocation() + WorldDirection * 0.5, BatterCamera->GetComponentLocation()	+ WorldDirection * 100, FColor::Purple, false, -1, 0, 0.6);
		GetWorld()->LineTraceSingleByObjectType(HitResult,
			BatterCamera->GetComponentLocation() + WorldDirection * 0.5,
			BatterCamera->GetComponentLocation() + WorldDirection * 100,
			ObjectParams,
			Params);
		if (HitResult.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Hit!"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Nothing hit :("));
		}
	}
	
	
}

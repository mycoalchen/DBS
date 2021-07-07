// Fill out your copyright notice in the Description page of Project Settings.


#include "PrecisionController.h"
#include "../Pitches/BallBase.h"
#include "../UI/SwingReticle.h"
#include "../UI/PrecisionTrainingSidebar.h"
#include "../Framework/MyGSB.h"
#include "Blueprint/UserWidget.h"
#include "Components/ContentWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"
#include "Components/SceneComponent.h"
#include "CineCameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/Image.h"

APrecisionController::APrecisionController()
{
	SwingPlane = FPlane(SwingPlanePoint1, SwingPlanePoint2, SwingPlanePoint3);
}

void APrecisionController::BeginPlay()
{
	Super::BeginPlay();
	CreateUI();
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
	if (SidebarClass)
	{
		Sidebar = CreateWidget<UPrecisionTrainingSidebar>(GetWorld(), SidebarClass, FName(TEXT("Sidebar")));
		Sidebar->AddToViewport();
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
	if (CanSwing)
	{
		CanSwing = false;
		if (!ActiveBall) return;
		Swing(SwingFrames);
		// Call this function on the next frame as well
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("Swing"), SwingFrames);
		GetWorldTimerManager().SetTimerForNextTick(TimerDelegate);
	}
}

void APrecisionController::Swing(int32 FramesRemaining)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Swung!"));
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
		const FVector SwingLocation = FMath::LinePlaneIntersection(WorldPosition, WorldPosition + WorldDirection, SwingPlane);
		DrawDebugSphere(GetWorld(), SwingLocation, SwingHitRadius, 10, FColor::Green, false, 2);
		DrawDebugSphere(GetWorld(), ActiveBall->GetActorLocation(), 7.8, 10, FColor::Red, false, 2);
		if (FVector::DistSquared(SwingLocation, ActiveBall->GetActorLocation()) <= SwingHitRadius * SwingHitRadius)
		{
			ActiveBall->Status = EBallStatus::BS_Hit;
			Sidebar->UpdateHit(true);
		}
		else if (FramesRemaining > 1)
		{
			// Call this function on the next frame as well
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("Swing"), FramesRemaining - 1);
			GetWorldTimerManager().SetTimerForNextTick(TimerDelegate);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("Called miss clause"));
			ActiveBall->Status = EBallStatus::BS_Strike;
			Sidebar->UpdateHit(false);
			Sidebar->UpdateMiss(ActiveBall->GetActorLocation() - SwingLocation, SwingHitRadius);
		}
	}
}


void APrecisionController::OnBallWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{
		switch (Ball->Status)
		{
		case EBallStatus::BS_Ball:
			Sidebar->UpdateCount(false);
			Sidebar->UpdateStrike(false); break;
		case EBallStatus::BS_Strike:
			Sidebar->UpdateCount(true);
			Sidebar->UpdateStrike(true); break;
		case EBallStatus::BS_Hit: break;
		}
		ActiveBall = nullptr;
		GetWorld()->DestroyActor(Ball);
		CanSwing = false;
	}
}

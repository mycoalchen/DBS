// Fill out your copyright notice in the Description page of Project Settings.


#include "PrecisionController.h"
#include "../Pitches/BallBase.h"
#include "../UI/SwingReticle.h"
#include "../UI/PrecisionTrainingSidebar.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/Image.h"

APrecisionController::APrecisionController()
{
}

void APrecisionController::BeginPlay()
{
	Super::BeginPlay();
	CreateUI();
	SwingPlane = FPlane(SwingPlanePoint1, SwingPlanePoint2, SwingPlanePoint3);
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
		// Call the CheckSwing function on the next frame
		FTimerDelegate TimerDelegate;
		CheckSwing(SwingDuration);
		Sidebar->UpdateSwing(true);
		ReticleSensitivity /= 10;
		SwingToBall = FVector(10000, 10000, 10000);
	}
}

void APrecisionController::CheckSwing(float TimeRemaining)
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
		// Calculate the swing location using the swing plane and deprojected click location
		const FVector SwingLocation = FMath::LinePlaneIntersection(WorldPosition, WorldPosition + WorldDirection, SwingPlane);
		const FVector NewSwingToBall = SwingLocation - ActiveBall->GetActorLocation();
		if (NewSwingToBall.SizeSquared() < SwingToBall.SizeSquared()) {
			SwingToBall = NewSwingToBall;
			BestSwingLocation = SwingLocation;
			BestBallLocation = ActiveBall->GetActorLocation();
		}
		/*DrawDebugSphere(GetWorld(), SwingLocation, SwingHitRadius, 10, FColor::Green, false, 2);
		DrawDebugLine(GetWorld(), SwingLocation, SwingLocation - FVector(0, 0, 200), FColor::Green, false, 2, 0, 0.3);
		DrawDebugSphere(GetWorld(), ActiveBall->GetActorLocation(), 7.8, 10, FColor::Red, false, 2);
		DrawDebugLine(GetWorld(), ActiveBall->GetActorLocation(), ActiveBall->GetActorLocation() - FVector(0, 0, 200), FColor::Red, false, 2, 0, 0.3);*/
		if (TimeRemaining <= 0)
		{
			ActiveBall->Status = EBallStatus::BS_Strike;
			Sidebar->UpdateHit(false);
			Sidebar->UpdateMiss(-SwingToBall, SwingHitRadius);
			ReticleSensitivity *= 10;
			DrawDebugSphere(GetWorld(), BestSwingLocation, SwingHitRadius, 16, FColor::Blue, false, 2);
			// DrawDebugLine(GetWorld(), BestSwingLocation, BestSwingLocation - FVector(0, 0, 200), FColor::Blue, false, 2, 0, 0.3);
			DrawDebugSphere(GetWorld(), BestBallLocation, 3.9, 16, FColor::Red, false, 2);
			// DrawDebugLine(GetWorld(), BestBallLocation, BestBallLocation - FVector(0, 0, 200), FColor::Red, false, 2, 0, 0.3);
			return;
		}
		if (FVector::DistSquared(SwingLocation, ActiveBall->GetActorLocation()) <= (SwingHitRadius+7.8)* (SwingHitRadius + 7.8))
		{
			ActiveBall->Status = EBallStatus::BS_Hit;
			Sidebar->UpdateHit(true);
			ReticleSensitivity *= 10;
			DrawDebugSphere(GetWorld(), BestSwingLocation, SwingHitRadius, 16, FColor::Blue, false, 2);
			// DrawDebugLine(GetWorld(), BestSwingLocation, BestSwingLocation - FVector(0, 0, 200), FColor::Blue, false, 2, 0, 0.3);
			DrawDebugSphere(GetWorld(), BestBallLocation, 3.9, 16, FColor::Red, false, 2);
			// DrawDebugLine(GetWorld(), BestBallLocation, BestBallLocation - FVector(0, 0, 200), FColor::Red, false, 2, 0, 0.3);
			return;
		}
		// Call this function on the next frame
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("CheckSwing"), TimeRemaining - GetWorld()->GetDeltaSeconds());
		GetWorldTimerManager().SetTimerForNextTick(TimerDelegate);
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
			Sidebar->UpdateSwing(false);
			Sidebar->UpdateStrike(false); break;
		case EBallStatus::BS_Strike:
			Sidebar->UpdateCount(true);
			if (!Sidebar->SwingText->GetText().EqualTo(FText::FromString("Swung"))) {
				Sidebar->UpdateSwing(false);
			}
			Sidebar->UpdateStrike(true); break;
		case EBallStatus::BS_Hit: 
			break;
		}
		ActiveBall = nullptr;
		GetWorld()->DestroyActor(Ball);
		// CanSwing = false;
	}
}

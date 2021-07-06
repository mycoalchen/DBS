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
#include "Components/SphereComponent.h"
#include "CineCameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/Image.h"

APrecisionController::APrecisionController()
{
	SwingSphere = CreateDefaultSubobject<USphereComponent>(FName("SwingSphere"));
	SwingSphere->SetSphereRadius(SwingSphereRadius);
	SwingSphere->SetRelativeLocation(FVector(-100, 0, 0));
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
	if (ActiveBall && IsSwinging)
	{
		FVector v = ActiveBall->GetActorLocation() - SwingSphere->GetComponentLocation();
		if (v.SizeSquared() < SphereToBall.SizeSquared()) SphereToBall = v;
	}
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
		IsSwinging = true;
		if (ActiveBall)
			ActiveBall->Status = EBallStatus::BS_Strike;
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

void APrecisionController::OnSwingSphereOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{
		Ball->Status = EBallStatus::BS_Hit;
		Sidebar->UpdateHit(true);
	}
}


void APrecisionController::OnSwingFinished()
{
	IsSwinging = false;
	SwingSphere->SetRelativeLocation(FVector(-100, 0, 0));
	GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
	if (!ActiveBall || ActiveBall->Status != EBallStatus::BS_Hit)
	{
		Sidebar->UpdateHit(false);
		Sidebar->UpdateMiss(SphereToBall, 7.8 + SwingSphereRadius);
	}
	SphereToBall = FVector(1000, 1000, 1000);
}

#pragma region PrecisionTrainingSidebar callers
void APrecisionController::UpdateCount(bool Strike)
{
	Sidebar->UpdateCount(Strike);
}
void APrecisionController::UpdatePitch(int Type, int SpeedMPH)
{
	Sidebar->UpdatePitch(Type, SpeedMPH);
}
#pragma endregion
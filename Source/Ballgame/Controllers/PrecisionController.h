// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "PrecisionController.generated.h"

UCLASS()
class BALLGAME_API APrecisionController : public APlayerCharacter
{
	GENERATED_BODY()

	APrecisionController();

public:
	UFUNCTION()
		void OnBallWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(BlueprintReadWrite, Category = "Swinging")
		bool CanSwing = true;
	// Number of seconds to keep swing sphere active
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingDuration = 0.08;
	
	// Sidebar for count, pitch, and swing information
	UPROPERTY(BlueprintReadWrite, Category = "UI")
		class UPrecisionTrainingSidebar* Sidebar;
	
protected:
	// Sets up reticle and other UI elements; called at beginning
	UFUNCTION()
		void CreateUI();

	// Input handlers
	UFUNCTION()
		void MouseX(float Value);
	UFUNCTION()
		void MouseY(float Value);
	UFUNCTION()
		void LeftClick();
	// Called every frame while swinging
	UFUNCTION()
		void CheckSwing(float SwingTimeRemaining);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	// Reticle used to aim at ball
	UPROPERTY(BlueprintReadWrite, Category = "UI")
		class USwingReticle* Reticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class USwingReticle> ReticleClass;
	// Current position of reticle
	UPROPERTY(BlueprintReadWrite, Category = "UI")
		float ReticleX = 0;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
		float ReticleY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		float ReticleSensitivity = 25;
	float MinReticleX, MinReticleY, MaxReticleX, MaxReticleY;
	
	// Indicates where the barrel should be swung for various pitches at different heights and y-coordinates
	UPROPERTY(BlueprintReadWrite, Category = "Swinging")
		FPlane SwingPlane;
	// Points defining what the swing plane should be
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		FVector SwingPlanePoint1 = FVector(21.5, -22, 166);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		FVector SwingPlanePoint2 = FVector(10, -10, 130);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		FVector SwingPlanePoint3 = FVector(-21.5, 22, 101);
	// If the swing is within SwingHitRadius of the ball, it counts as a hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingHitRadius = 17.5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class TSubclassOf<class UPrecisionTrainingSidebar> SidebarClass;
	
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingSphereRadius = 9.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingSphereDuration = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingSphereXDistance = 196;

	// Sidebar for count, pitch, and swing information
	UPROPERTY(BlueprintReadWrite, Category = "UI")
		class UPrecisionTrainingSidebar* Sidebar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class TSubclassOf<class UPrecisionTrainingSidebar> SidebarClass;
	
	// Vector from ball to swing sphere- updated when swing sphere and ball are active
	FVector VectorToBall;
	FTimerHandle SwingTimerHandle;
	UFUNCTION()
		void OnSwingSphereOverlapped(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when the swing sphere's duration ends
	UFUNCTION()
		void OnSwingFinished();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		class USphereComponent* SwingSphere;
	
};

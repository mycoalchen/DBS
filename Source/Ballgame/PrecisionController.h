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
	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		class USwingReticle* Reticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reticle")
		TSubclassOf<class USwingReticle> ReticleClass;
	// Current position of reticle
	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		float ReticleX = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		float ReticleY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reticle")
		float ReticleSensitivity = 25;
	float MinReticleX, MinReticleY, MaxReticleX, MaxReticleY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingSphereRadius = 9.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingSphereDuration = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		float SwingSphereXDistance = 196;
	// Whether the swing sphere is currently in position
	bool IsSwinging = false;
	FVector VectorToBall;
	FTimerHandle SwingTimerHandle;
	UFUNCTION()
		void OnSwingSphereOverlapped(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when the swing sphere's duration ends
	UFUNCTION()
		void OnSwingFinished();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging")
		class USphereComponent* SwingSphere;

public:
	// Ball most recently thrown and still in the air
	UPROPERTY(BlueprintReadWrite);
	class ABallBase* ActiveBall;
	
};

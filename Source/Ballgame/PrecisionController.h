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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Swinging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swinging", meta=(MakeEditWidget="true"))
		class USceneComponent* SwingStartPoint;
	
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
};

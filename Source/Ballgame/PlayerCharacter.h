// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BALLGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* BatterCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float LookSensitivity = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float EyeHeight = 73;

#pragma region Training UI
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mouse move reticle
	UFUNCTION()
		void MouseVertical(float Value);
	UFUNCTION()
		void MouseHorizontal(float Value);
};

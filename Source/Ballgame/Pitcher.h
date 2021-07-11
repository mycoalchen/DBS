// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pitcher.generated.h"

UCLASS()
class BALLGAME_API APitcher : public APawn
{
	GENERATED_BODY()
	
public:	
	APitcher();

	virtual void Tick(float DeltaTime) override;

	// Maximum angle the throw will be off by
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwing")
		float InaccuracyRadians = 0.025;

	// Number of seconds between throws
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwing")
		float TimeBetweenThrows = 2.5;

	// Number of seconds since last throw
	float TimeSinceLastThrow = 0;

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* Body_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capsule")
		class UCapsuleComponent* Capsule;
	
	// Point to throw ball from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		class USceneComponent* ReleasePoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pitch types")
		TSubclassOf<class AFastball> FastballClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pitch types")
		TSubclassOf<class ACurveball> CurveballClass;
	
	// Initial xy-speed of fastballs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		float FastballSpeedMPH = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		int FastballSpinRateRPM = 2200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		FRotator FastballStartRotator = FRotator(0, 90, 0);
	UFUNCTION(BlueprintCallable, Category = "Pitch functions")
		void ThrowFastball(float MPH, float SpinRate);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		FRotator CurveballStartRotator = FRotator(0, 30, -90);
	UFUNCTION(BlueprintCallable, Category = "My functions")
		void ThrowCurveball(float MPH, float SpinRate);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animation")
		void PlayPitchAnimation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float ReleaseDelay = 0.7; // Time after animation starts to release pitch

protected:
	virtual void BeginPlay() override;

private:
	// Called when the timer in ThrowFastball finishes (so the ball is thrown at the correct point in the animation)
	UFUNCTION()
		void ThrowFastball2(float MPH, float SpinRate);
	UFUNCTION()
		void ThrowCurveball2(float MPH, float SpinRate);
	
};

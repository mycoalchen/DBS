// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pitcher.generated.h"

UCLASS()
class BALLGAME_API APitcher : public AActor
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* StaticMesh;
	
	// Point to throw ball from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		class USceneComponent* ReleasePoint;

	// Target to throw to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		class USceneComponent* PitchTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch types")
		TSubclassOf<class AFastball> FastballClass;

	// Initial xy-speed of fastballs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		float FastballSpeedMPH = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch stats")
		int FastballSpinRateRPM = 2200;

	UFUNCTION(BlueprintCallable, Category = "My functions")
		void ThrowFastball();
	
protected:
	virtual void BeginPlay() override;
	
};

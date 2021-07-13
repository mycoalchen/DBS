// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(Blueprintable)
enum class EBallStatus : uint8
{
	BS_Strike	UMETA(DisplayName = "Strike"),
	BS_Ball		UMETA(DisplayName = "Ball"),
	BS_Hit		UMETA(DisplayName = "Hit"),
};

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BallBase.generated.h"

UCLASS()
class BALLGAME_API ABallBase : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	// Sets default values for this actor's properties
	ABallBase();

	// Release information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Release Info")
		int SpeedMPH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Release Info")
		int PitchType; // 1 fastball, 2 curveball
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMesh* BlurStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		float StaticMeshOpacity = 0.9;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		UProjectileMovementComponent* PMC;

	// Pure virtual function for physics, called every frame
	virtual void PhysicsTick() PURE_VIRTUAL(ABallBase::PhysicsTick);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin")
		float SpinRateRPM = 2300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin")
		FRotator SpinRotator = FRotator(0, 0, 0);

	// Array of translucent static meshes used for motion blur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		TArray<UStaticMeshComponent*> BlurMeshes;
	// Number of blur meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		int32 NumBlurMeshes = 20;
	// Angle separating each blur mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		float BlurMeshAngle = -2;
	// Opacity of leading blur mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		float BlurMeshMaxOpacity = 0.3;
	// Translucent ball (white) material used on blur meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		class UMaterial* BallTranslucentMaterial;
	// Translucent strap (red) material used on blur meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		class UMaterial* StrapTranslucentMaterial;
	// Float curve to use for opacity values - must pass through (1,0) and (0,1) without negative values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		class UCurveFloat* OpacityFloatCurve;
	// Whether this ball is strike, ball, or hit
	EBallStatus Status = EBallStatus::BS_Ball;
	
};
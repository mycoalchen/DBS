// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		float StaticMeshOpacity = 0.9;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		class UProjectileMovementComponent* PMC;

	// Pure virtual function for physics, called every frame
	virtual void PhysicsTick() PURE_VIRTUAL(ABallBase::PhysicsTick);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin")
		float SpinRateRPM = 2300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin")
		FRotator SpinRotator = FRotator(0, 0, 0);

	// Instanced static mesh used to instance many translucent meshes for motion blur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		class UInstancedStaticMeshComponent* BlurMesh;
	// Number of blur mesh instances
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		int32 NumBlurMeshes = 20;
	// Angle separating each blur mesh instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Blur")
		float BlurMeshAngle = -2;
	// Opacity of leading blur mesh instance
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
	// Whether this ball hit the strike zone
	bool Strike = false;
	
};

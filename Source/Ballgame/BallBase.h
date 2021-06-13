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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ABallBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		class UProjectileMovementComponent* PMC;

	// Pure virtual function for physics, called every frame
	virtual void PhysicsTick() PURE_VIRTUAL(ABallBase::PhysicsTick);

};

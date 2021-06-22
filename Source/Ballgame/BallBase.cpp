// Fill out your copyright notice in the Description page of Project Settings.

#include "BallBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABallBase::ABallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	StaticMesh->SetMobility(EComponentMobility::Movable);
	SetRootComponent(StaticMesh);

	BlurMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("BlurMesh"));
	BlurMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlurMesh->SetupAttachment(StaticMesh);
	
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(FName("PMC"));
	PMC->ProjectileGravityScale = 1;

}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();
	BlurMesh->RegisterComponent();
	// Instance the blur meshes
	if (BlurMesh && BlurMesh->GetStaticMesh() && BallTranslucentMaterial && StrapTranslucentMaterial && OpacityFloatCurve)
	{
		for (int32 i = 0; i < NumBlurMeshes; i++)
		{
			float Opacity = BlurMeshMaxOpacity * OpacityFloatCurve->GetFloatValue(i / NumBlurMeshes);
			if (i == 16)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Blue, FString::SanitizeFloat(Opacity));
			}
			BlurMesh->AddInstance(FTransform(i * SpinRotator * BlurMeshAngle, FVector(0, 0, 0), FVector(1, 1, 1)));
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::SanitizeFloat(i * SpinRotator.Pitch * BlurMeshAngle));
			UMaterialInstanceDynamic* BallMaterial = BlurMesh->CreateDynamicMaterialInstance(0, BallTranslucentMaterial);
			BallMaterial->SetScalarParameterValue(FName("Opacity"), Opacity);
			UMaterialInstanceDynamic* StrapMaterial = BlurMesh->CreateDynamicMaterialInstance(1, StrapTranslucentMaterial);
			StrapMaterial->SetScalarParameterValue(FName("Opacity"), Opacity);
		}
	}
	else if (!BlurMesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Dafuq"));
	}
	else if (!BlurMesh->GetStaticMesh())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("What the fuc"));
	}
}

void ABallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(SpinRotator * SpinRateRPM * 6 * DeltaTime); // conversion factor from rpm to d/sec = 6
	PhysicsTick();
}

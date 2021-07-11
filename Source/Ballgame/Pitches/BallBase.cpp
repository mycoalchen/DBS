// Fill out your copyright notice in the Description page of Project Settings.

#include "BallBase.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABallBase::ABallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	SetRootComponent(StaticMeshComponent);
	
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(FName("PMC"));
	PMC->ProjectileGravityScale = 1;

}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();
	if (StaticMeshComponent)
	{
		UMaterialInstanceDynamic* BallMaterial = StaticMeshComponent->CreateDynamicMaterialInstance(0, BallTranslucentMaterial);
		UMaterialInstanceDynamic* StrapMaterial = StaticMeshComponent->CreateDynamicMaterialInstance(1, StrapTranslucentMaterial);
		BallMaterial->SetScalarParameterValue(FName("Opacity"), StaticMeshOpacity);
		StrapMaterial->SetScalarParameterValue(FName("Opacity"), StaticMeshOpacity);
	}

	// Instance the blur meshes
	if (BallTranslucentMaterial && StrapTranslucentMaterial && OpacityFloatCurve && BlurStaticMesh)
	{
		for (int32 i = 0; i < NumBlurMeshes; i++)
		{
			UStaticMeshComponent* BlurMesh = NewObject<UStaticMeshComponent>(this);
			BlurMesh->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			BlurMesh->RegisterComponent();
			BlurMesh->SetStaticMesh(BlurStaticMesh);
			BlurMeshes.Add(BlurMesh);
			BlurMesh->SetRelativeTransform(FTransform(FRotator(i * SpinRotator * BlurMeshAngle), FVector(0, 0, 0), FVector(1, 1, 1)));
			const float Opacity = BlurMeshMaxOpacity * OpacityFloatCurve->GetFloatValue(float(i) / float(NumBlurMeshes));
			// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::SanitizeFloat(Opacity));
			UMaterialInstanceDynamic* BallMaterial = BlurMesh->CreateDynamicMaterialInstance(0, BallTranslucentMaterial);
			BallMaterial->SetScalarParameterValue(FName("Opacity"), Opacity);
			UMaterialInstanceDynamic* StrapMaterial = BlurMesh->CreateDynamicMaterialInstance(1, StrapTranslucentMaterial);
			StrapMaterial->SetScalarParameterValue(FName("Opacity"), Opacity);
		}
	}
	if (!BallTranslucentMaterial) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("BallTranslucentMaterial null"));
	if (!StrapTranslucentMaterial) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("StrapTranslucentMaterial null"));
	if (!OpacityFloatCurve) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("OpacityFloatCurve null"));
	if (!BlurStaticMesh) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("BlurStaticMesh null"));
}

void ABallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// AddActorWorldRotation(SpinRotator * SpinRateRPM * 6 * DeltaTime); // conversion factor from rpm to d/sec = 6
	StaticMeshComponent->AddLocalRotation(SpinRotator * SpinRateRPM * 6 * DeltaTime); // conversion factor from rpm to d/sec = 6
	PhysicsTick();
}

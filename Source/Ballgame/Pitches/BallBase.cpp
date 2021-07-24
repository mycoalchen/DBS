// Fill out your copyright notice in the Description page of Project Settings.

#include "BallBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h"

// Sets default values
ABallBase::ABallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	SetRootComponent(StaticMeshComponent);
	
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(FName("PMC"));
	
}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();
	
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
			BlurMesh->SetRelativeTransform(FTransform(FRotator(i * SpinRotator * -BlurMeshAngle), FVector(0, 0, 0), FVector(1, 1, 1)));
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
	
	AddActorLocalRotation(SpinRotator * SpinRateRPM * 6 * DeltaTime); // conversion factor from rpm to d/sec = 6
	// StaticMeshComponent->AddLocalRotation(SpinRotator * SpinRateRPM * 6 * DeltaTime); // conversion factor from rpm to d/sec = 6
	PhysicsTick();
	
	// DrawDebugLine(GetWorld(), PrevLocation, GetActorLocation(), FColor::Red, false, 1.5);
	// DrawDebugSphere(GetWorld(), GetActorLocation(), 0.5, 10, FColor::Red, false, 1.5);
	const FVector DirectionVector = GetActorLocation() - PrevLocation;
	const FQuat DirectionQuat = FQuat::FindBetweenVectors(FVector(0, 0, 1), DirectionVector);
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, DirectionVector.ToOrientationQuat().ToString());
	PrevLocation = GetActorLocation();

	// DrawDebugCapsule(GetWorld(), GetActorLocation(), 16, 3.9, DirectionQuat, FColor::Red, false, 1.5);
}

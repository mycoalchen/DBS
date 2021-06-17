// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "CinematicCamera/Public/CineCameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BatterCamera = CreateDefaultSubobject<UCineCameraComponent>(FName("BatterCamera"));
	BatterCamera->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Mouse look
	PlayerInputComponent->BindAxis("LookHorizontal", this, &APlayerCharacter::MouseHorizontal);
	PlayerInputComponent->BindAxis("LookVertical", this, &APlayerCharacter::MouseVertical);
}

// Mouse look
void APlayerCharacter::MouseHorizontal(float Value)
{
	
}
void APlayerCharacter::MouseVertical(float Value)
{
	
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "Strikezone.h"
#include "Components/BoxComponent.h"
#include "BallBase.h"
#include "MyGSB.h"
#include "DrawDebugHelpers.h"

// Sets default values
AStrikezone::AStrikezone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(FName("OverlapBox"));
	if (StrikezoneBottom >= StrikezoneTop)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Strikezone bottom is above strikezone top!"));
	}
	OverlapBox->SetBoxExtent(FVector(21.59, 21.59, 50 * (StrikezoneTop - StrikezoneBottom)));
	OverlapBox->SetRelativeScale3D(FVector(1, 1, 1));
	OverlapBox->ShapeColor = FColor::Green;
	OverlapBox->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void AStrikezone::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AStrikezone::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AStrikezone::OnOverlapEnd);
	AMyGSB* GameState = Cast<AMyGSB>(GetWorld()->GetGameState());
	if (GameState)
		GameState->Strikezone = this;
}

void AStrikezone::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AStrikezone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, TEXT("Strike!"));
	}
}

void AStrikezone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{}
}

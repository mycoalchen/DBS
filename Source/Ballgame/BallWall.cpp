// Fill out your copyright notice in the Description page of Project Settings.


#include "BallWall.h"
#include "BallBase.h"
#include "MyGSB.h"
#include "Components/BoxComponent.h"

void ABallWall::BeginPlay()
{
	OverlapBox->OnComponentBeginOverlap.RemoveAll(this);
	AMyGSB* GameState = Cast<AMyGSB>(GetWorld()->GetGameState());
	if (GameState)
		GameState->BallWall = this;
	
}

void ABallWall::OnOverlapBegin2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{
		
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BallWall.h"
#include "BallBase.h"
#include "MyGSB.h"
#include "MyGI.h"
#include "PrecisionController.h"
#include "Components/BoxComponent.h"

void ABallWall::BeginPlay()
{
	OverlapBox->OnComponentBeginOverlap.RemoveAll(this);
	AMyGSB* GameState = Cast<AMyGSB>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->BallWall = this;
		UMyGI* GameInstance = Cast<UMyGI>(GetGameInstance());
		if (GameInstance)
		{
			switch (GameInstance->InputMode)
			{
			case EInputMode::IM_Precision:
				APrecisionController* PC = Cast<APrecisionController>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				if (PC)
				{
					GameState->PlayerCharacter = PC; // Have to call this here to ensure that the following code works (BeginPlay may be called on this actor first)
					OverlapBox->OnComponentBeginOverlap.AddDynamic(PC, &APrecisionController::OnBallWallHit);
				}
			}
		}
	}
}
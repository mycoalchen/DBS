// Fill out your copyright notice in the Description page of Project Settings.


#include "BallWall.h"
#include "Pitches/BallBase.h"
#include "Framework/MyGI.h"
#include "Framework/MyGSB.h"
#include "Controllers/PrecisionController.h"
#include "Controllers/ZoneController.h"
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
					GameState->PlayerCharacter = PC;
					OverlapBox->OnComponentBeginOverlap.AddDynamic(PC, &APrecisionController::OnBallWallHit);
				}
			case EInputMode::IM_Zone:
				AZoneController* ZC = Cast<AZoneController>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				if (ZC)
				{
					GameState->PlayerCharacter = ZC;
					OverlapBox->OnComponentBeginOverlap.AddDynamic(ZC, &AZoneController::OnBallWallHit);
				}
			}
		}
	}
}
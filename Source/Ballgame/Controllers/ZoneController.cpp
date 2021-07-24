// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneController.h"

void AZoneController::OnBallWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABallBase* Ball = Cast<ABallBase>(OtherActor);
	if (Ball)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Ballwall hit with ball"));
		Sidebar->UpdatePitch(Ball->PitchType, Ball->SpeedMPH);
		switch (Ball->Status)
		{
		case EBallStatus::BS_Ball:
			Sidebar->UpdateCount(false);
			Sidebar->UpdateSwing(false);
			Sidebar->UpdateStrike(false); break;
		case EBallStatus::BS_Strike:
			Sidebar->UpdateCount(true);
			if (!Sidebar->SwingText->GetText().EqualTo(FText::FromString("Swung"))) {
				Sidebar->UpdateSwing(false);
			}
			Sidebar->UpdateStrike(true); break;
		case EBallStatus::BS_Hit:
			break;
		}
		ActiveBall = nullptr;
		GetWorld()->DestroyActor(Ball);
		// CanSwing = false;
	}
}

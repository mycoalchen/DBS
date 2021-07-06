// Fill out your copyright notice in the Description page of Project Settings.


#include "PrecisionTrainingSidebar.h"
#include "Components/TextBlock.h"

void UPrecisionTrainingSidebar::NativeConstruct()
{
	for (UTextBlock* Block : {StrikeText, LocationText, PitchText, HitText, MissText, ResultText})
	{
		Block->SetText(FText::FromString(FString("")));
	}
	CountText->SetText(FText::FromString(FString("0-0")));
}

#pragma region Count and result
void UPrecisionTrainingSidebar::SetResult(int result)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ClearResult"));
	switch (result)
	{
	case 1:
		ResultText->SetText(FText::FromString("Struck out")); break;
	case 2:
		ResultText->SetText(FText::FromString("Walked")); break;
	case 3:
		ResultText->SetText(FText::FromString("Hit")); break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("SetResult result param error in PrecisionTrainingSidebar"));
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ResultDisplayTime, false);
}

void UPrecisionTrainingSidebar::ClearResult()
{
	Strikes = Balls = 0;
	ResultText->SetText(FText::FromString(""));
	CountText->SetText(FText::FromString("0-0"));
}

void UPrecisionTrainingSidebar::UpdateCount(bool Strike)
{
	Strikes += Strike;
	Balls += !Strike;
	TArray< FStringFormatArg > args;
	args.Add(FStringFormatArg(Balls));
	args.Add(FStringFormatArg(Strikes));
	CountText->SetText(FText::FromString(FString::Format(TEXT("{0}-{1}"), args)));
	if (Strikes == 3)
		SetResult(1);
	else if (Balls == 4)
		SetResult(2);
}
#pragma endregion

void UPrecisionTrainingSidebar::UpdatePitch(int Type, int SpeedMPH)
{
	TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(SpeedMPH));
	switch (Type)
	{
	case 1:
		args.Add(FStringFormatArg("4 Seam Fastball")); break;
	case 2:
		args.Add(FStringFormatArg("Curveball")); break;
	default:
		args.Add(FStringFormatArg("PrecisionTrainingSidebar UpdatePitch Type error")); break;
	}
	PitchText->SetText(FText::FromString(FString::Format(TEXT("{1}-\r{0} mph"), args)));
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ClearPitch"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, PitchDisplayTime, false);
}

void UPrecisionTrainingSidebar::ClearPitch()
{
	PitchText->SetText(FText::FromString(""));
}

void UPrecisionTrainingSidebar::UpdateStrike(bool Strike)
{
	if (Strike) StrikeText->SetText(FText::FromString("Strike"));
	else StrikeText->SetText(FText::FromString("Ball"));
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ClearStrike"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, StrikeDisplayTime, false);
}

void UPrecisionTrainingSidebar::ClearStrike()
{
	StrikeText->SetText(FText::FromString(""));
}

void UPrecisionTrainingSidebar::UpdateHit(bool Hit)
{
	if (Hit) {
		HitText->SetText(FText::FromString("Hit"));
		SetResult(3);
	}
	else HitText->SetText(FText::FromString("Missed"));
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ClearHit"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, HitDisplayTime, false);
}

void UPrecisionTrainingSidebar::ClearHit()
{
	HitText->SetText(FText::FromString(""));
}


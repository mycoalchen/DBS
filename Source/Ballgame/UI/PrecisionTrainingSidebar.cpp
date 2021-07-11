// Fill out your copyright notice in the Description page of Project Settings.


#include "PrecisionTrainingSidebar.h"
#include "Components/TextBlock.h"

void UPrecisionTrainingSidebar::NativeConstruct()
{
	for (UTextBlock* Block : {StrikeText, LocationText, PitchText, HitText, MissText, ResultText, SwingText})
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

// v is the shortest vector from the swing sphere to the ball over the duration of the swing
void UPrecisionTrainingSidebar::UpdateMiss(FVector v, float m)
{
	FString s = FString();
	if (v.Z > m)
		s = "Low";
	if (v.Z < -m)
		s = "High";
	if (v.Y > m)
	{
		if (!s.IsEmpty()) s += "/Left";
		else s = "Left";
	}
	if (v.Y < -m)
	{
		if (!s.IsEmpty()) s += "/Right";
		else s = "Right";
	}
	if (v.X > m)
	{
		if (!s.IsEmpty()) s += "/Early";
		else s = "Early";
	}
	if (v.X < -m)
	{
		if (!s.IsEmpty()) s += "/Late";
		else s = "Late";
	}
	MissText->SetText(FText::FromString(s));
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ClearMiss"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, HitDisplayTime, false);
}

void UPrecisionTrainingSidebar::ClearMiss()
{
	MissText->SetText(FText::FromString(""));
}

void UPrecisionTrainingSidebar::UpdateSwing(bool Swung)
{
	if (Swung) SwingText->SetText(FText::FromString("Swung"));
	else SwingText->SetText(FText::FromString("Taken"));
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ClearSwing"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ResultDisplayTime, false);
}

void UPrecisionTrainingSidebar::ClearSwing()
{
	SwingText->SetText(FText::FromString(""));
}

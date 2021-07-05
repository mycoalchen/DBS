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
		ResultText->SetText(FText::FromString("Got a hit")); break;
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

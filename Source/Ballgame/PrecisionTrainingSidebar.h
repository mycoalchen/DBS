// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PrecisionTrainingSidebar.generated.h"

/**
 * 
 */
UCLASS()
class BALLGAME_API UPrecisionTrainingSidebar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* StrikeText; // "STRIKE" or "BALL"
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* LocationText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* CountText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* PitchText; // Pitch type and speed
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* HitText; // "HIT" or "MISS"
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MissText; // Where the swing missed
};

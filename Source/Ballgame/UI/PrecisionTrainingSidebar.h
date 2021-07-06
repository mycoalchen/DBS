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
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ResultText;
	// Time to display the parts of the sidebar
	UPROPERTY(BlueprintReadWrite)
		float ResultDisplayTime = 2;
	UPROPERTY(BlueprintReadWrite)
		float PitchDisplayTime = 2;
	UPROPERTY(BlueprintReadWrite)
		float StrikeDisplayTime = 1;

	// Adds either a strike or a ball to the count
	UFUNCTION()
		void UpdateCount(bool Strike);
	// Updates the pitch display - 1 fastball, 2 curveball
	UFUNCTION()
		void UpdatePitch(int Type, int SpeedMPH);
	// Updates the strike display
	UFUNCTION()
		void UpdateStrike(bool Strike);
	
protected:
	// Set result text for one second- 1 strikeout, 2 walk, 3 hit
	UFUNCTION()
		void SetResult(int result);
	// Reset count and clear the result text
	UFUNCTION()
		void ClearResult();
	// Clear various text boxes
	UFUNCTION()
		void ClearPitch();
	UFUNCTION()
		void ClearStrike();
	
	virtual void NativeConstruct() override;
	
	// Strikes and balls
	int32 Strikes = 0, Balls = 0;
};

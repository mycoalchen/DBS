// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SwingReticle.generated.h"

UCLASS()
class BALLGAME_API USwingReticle : public UUserWidget
{
	GENERATED_BODY()

public:
	USwingReticle(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Changes color
	UFUNCTION()
		void SetActive(bool Active);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ReticleImage;

	// Reticle images
	UPROPERTY(EditAnywhere, Category = "Images")
		UObject* UnactiveReticleImage;
	UPROPERTY(EditAnywhere, Category = "Images")
		UObject* ActiveReticleImage;

	// tuned for 1920x1080 resolution
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		float PositionX = 1110;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		float PositionY = 640;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		float SizeX = 300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		float SizeY = 300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		float MaxPositionY = 880;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
		float MinPositionY = 200;
};

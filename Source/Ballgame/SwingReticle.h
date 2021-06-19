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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ReticleImage;

};

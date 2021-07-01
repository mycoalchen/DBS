// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGMB.h"
#include "MyGI.generated.h"

/**
 * 
 */
UCLASS()
class BALLGAME_API UMyGI : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input mode")
		EInputMode InputMode = EInputMode::IM_Precision;
	
};

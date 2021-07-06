// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGMB.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	IM_Precision	UMETA(DisplayName="Precision"),
	IM_Zone			UMETA(DisplayName="Zone"),
};

UCLASS()
class BALLGAME_API AMyGMB : public AGameModeBase
{
	GENERATED_BODY()
};

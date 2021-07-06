// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGSB.generated.h"

UCLASS()
class BALLGAME_API AMyGSB : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Objects")
		class AStrikezone* Strikezone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Objects")
		class APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Objects")
		class ABallWall* BallWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Objects")
		class APitcher* Pitcher;
};

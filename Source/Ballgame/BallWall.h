// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strikezone.h"
#include "BallWall.generated.h"

UCLASS()
class BALLGAME_API ABallWall : public AStrikezone
{
	GENERATED_BODY()

		virtual void BeginPlay() override;

};

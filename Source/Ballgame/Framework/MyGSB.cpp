// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGSB.h"

void AMyGSB::BeginPlay()
{
	Super::BeginPlay();
	if (Strikezone)
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("GSB Strikezone initialized"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("GSB Strikezone null"));
	if (Pitcher)
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("GSB Pitcher initialized"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("GSB Pitcher null"));
	if (BallWall)
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("GSB BallWall initialized"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("GSB BallWall null"));
	if (PlayerCharacter)
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("GSB PlayerCharacter initialized"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("GSB PlayerCharacter null"));
}

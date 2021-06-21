// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Strikezone.generated.h"

UCLASS()
class BALLGAME_API AStrikezone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrikezone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* OverlapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrikezoneTop = 1.042; // height, in meters, of top of strikezone
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrikezoneBottom = 0.53; // height, in meters, of bottom of strikezone
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the overlapbox is overlapped
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void PostInitializeComponents() override;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingReticle.h"
#include "Widgets/Layout/Anchors.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

USwingReticle::USwingReticle(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{

}

void USwingReticle::NativeConstruct()
{
	Super::NativeConstruct();
	
}
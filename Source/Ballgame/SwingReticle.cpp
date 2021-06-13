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
	if (UCanvasPanelSlot* s = Cast<UCanvasPanelSlot>(ReticleImage->Slot))
	{
		s->SetAnchors(FAnchors(0, 0, 0, 0));
		s->SetAlignment(FVector2D(0.5, 0.5));
		s->SetPosition(FVector2D(PositionX, PositionY));
		s->SetSize(FVector2D(SizeX, SizeY));
	}
}

void USwingReticle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UCanvasPanelSlot* s = Cast<UCanvasPanelSlot>(ReticleImage->Slot))
	{
		s->SetPosition(FVector2D(PositionX, PositionY));
	}
}

void USwingReticle::SetActive(bool Active)
{
	if (Active && ActiveReticleImage)
	{
		ReticleImage->SetBrushResourceObject(ActiveReticleImage);
	}
	else if (UnactiveReticleImage)
	{
		ReticleImage->SetBrushResourceObject(UnactiveReticleImage);
	}
}

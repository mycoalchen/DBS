// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "SwingReticle.h"

AMyHUD::AMyHUD()
{

}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (SwingReticleWidgetClass)
	{
		SwingReticleWidget = CreateWidget<USwingReticle>(GetWorld(), SwingReticleWidgetClass);
		if (SwingReticleWidget)
		{
			SwingReticleWidget->AddToViewport();
		}
	}
}

void AMyHUD::DrawHUD()
{

}

void AMyHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


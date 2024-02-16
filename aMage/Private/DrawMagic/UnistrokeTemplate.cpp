// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawMagic/UnistrokeTemplate.h"

#include "GameplayTagContainer.h"

FUnistrokeTemplate::FUnistrokeTemplate()
{
	Name = "";
	NameTag = FGameplayTag();
	Vector = TArray<float>();
	Points = TArray<FUnistrokePoint>();
}

FUnistrokeTemplate::FUnistrokeTemplate(const FString &Name,const FGameplayTag& NameTag, const TArray<FUnistrokePoint> &Points)
{
	this->Name = Name;
	this->NameTag = NameTag;
	this->Points = Points;

	FUnistrokePoint::Resample(this->Points, NumPoints);

	float Radians = FUnistrokePoint::IndicativeAngle(Points);

	FUnistrokePoint::RotateBy(this->Points, -Radians);

	FUnistrokePoint::ScaleTo(this->Points, SquareSize);

	FUnistrokePoint::TranslateTo(this->Points, FUnistrokePoint());

	this->Vector = FUnistrokePoint::Vectorize(this->Points);
}

FUnistrokeTemplate::~FUnistrokeTemplate()
{
}

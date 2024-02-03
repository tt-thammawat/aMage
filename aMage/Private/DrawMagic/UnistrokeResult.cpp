// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawMagic/UnistrokeResult.h"

FUnistrokeResult::FUnistrokeResult()
{
	this->Name = "No match";
	this->Score = 0.0f;
	this->Time = 0.0f;
}

FUnistrokeResult::FUnistrokeResult(const FString& Name, const float& Score, const float& Time)
{
	this->Name = Name;
	this->Score = Score;
	this->Time = Time;
}

FUnistrokeResult::~FUnistrokeResult()
{
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GAS_SLICE_API ConvertLibrary
{
public:
	ConvertLibrary();
	~ConvertLibrary();

	static bool ConvertFloatToBoolNegativePositiveRange(float Value);
};

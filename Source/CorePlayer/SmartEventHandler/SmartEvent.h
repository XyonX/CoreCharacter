// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



typedef void (*FunctionPointer)(int); // Typedef for a function pointer type

enum EFunctionCallFrequency
{
	
};

class COREPLAYER_API FSmartEvent
{
public:
	FSmartEvent()
	{
		
	};
	FSmartEvent(FunctionPointer Function)
	{
		
	};
	
	~FSmartEvent();

public:
	void Trigger ()
	{
		
	};
	void SetFunctionCallFrequency ()
	{
		
	};
};

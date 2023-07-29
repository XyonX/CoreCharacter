// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COREPLAYER_API ACorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// for input bindings
	virtual void SetupInputComponent() override;





	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CorePlayer/Animation/CoreAnimInstance.h"
#include "UObject/Object.h"
#include "AnimationDataManager.generated.h"

/**
 * 
 */
UCLASS()
class COREPLAYER_API UAnimationDataManager : public UObject
{
	GENERATED_BODY()

public:
	UAnimationDataManager() ;
	void Init(UCoreAnimInstance*InAnimInstance,ACoreCharacter*InPossessedCharacter,ACorePlayerController*InPlayerController);


	//Receiver  function
	void Receiver_LocomotionDataCalculator ();
	void Receiver_RotationalDataCalculator ();

private:

	UPROPERTY()
	UCoreAnimInstance*CoreAnimInstance;
	UPROPERTY()
	ACoreCharacter*CoreCharacter;
	UPROPERTY()
	ACorePlayerController*CoreController;
	UPROPERTY()
	UCharacterMovementComponent*CharacterMovementComponent;
	
};

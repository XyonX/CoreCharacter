// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationDataManager.h"

#include "CorePlayer/Animation/Threads/AnimationCalculator.h"
#include "CorePlayer/Player/CorePlayerController.h"
#include "CorePlugin/Helpers/DelegateHelper.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimationDataManager::UAnimationDataManager()

{
	
}

void UAnimationDataManager::Init(UCoreAnimInstance* InAnimInstance, ACoreCharacter* InPossessedCharacter,ACorePlayerController*InPlayerController)
{
	CoreAnimInstance=InAnimInstance;
	CoreCharacter=InPossessedCharacter;
	CoreController=InPlayerController;
	CharacterMovementComponent=CoreCharacter->GetCharacterMovement();

	ADelegateHelper::OnMovement.AddDynamic(this,&UAnimationDataManager::Receiver_LocomotionDataCalculator);
	ADelegateHelper::OnMovement.AddDynamic(this,&UAnimationDataManager::Receiver_RotationalDataCalculator);
	
}

void UAnimationDataManager::Receiver_LocomotionDataCalculator()
{
	CoreAnimInstance->CriticalSection.Lock();
	CoreAnimInstance->RawAnimationData.Velocity =CharacterMovementComponent->Velocity;
	CoreAnimInstance->CriticalSection.Unlock();
	CoreAnimInstance->AnimationCalculator->Event_OnVelocityChange->Trigger();
	CoreAnimInstance->GamethreadDataCalculator_Velocity();
}

void UAnimationDataManager::Receiver_RotationalDataCalculator()
{
	CoreAnimInstance->CriticalSection.Lock();
	CoreAnimInstance->RawAnimationData.ControllerRotation = CoreController->GetControlRotation();
	CoreAnimInstance->RawAnimationData.CharacterRotation =CoreCharacter->GetActorRotation();
	CoreAnimInstance->CriticalSection.Unlock();
	CoreAnimInstance->AnimationCalculator->Event_OnVelocityChange->Trigger();
	
}

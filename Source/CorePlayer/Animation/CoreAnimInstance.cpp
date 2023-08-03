/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreAnimInstance.h"
#include "Proxy/CoreAnimInstanceProxy.h"
#include "CorePlugin/Helpers/DelegateHelper.h"

UCoreAnimInstance::UCoreAnimInstance()
: CurrentAnimationData() // Initializes the CurrentAnimationData struct with default values (e.g., 0.0f and false)

{

	CurrentAnimationData=DefaultAnimationData;
}

void UCoreAnimInstance::NativeInitializeAnimation()
{
	
	Proxy = MakeShared<FCoreAnimInstanceProxy>(this);
	// Send a reference of the anim data to the proxy class
	Proxy->UpdateAnimationData(CurrentAnimationData);

	ADelegateHelper::Transmitter_AnimationData.AddDynamic(this,&UCoreAnimInstance::Receiver_AnimationData);
	
	ADelegateHelper::Transmitter_Velocity.AddDynamic(this,&UCoreAnimInstance::Receiver_Velocity);
	ADelegateHelper::Transmitter_CharacterWorldLocation.AddDynamic(this,&UCoreAnimInstance::Receiver_CharacterWorldLocation);
	ADelegateHelper::Transmitter_MovementDirection.AddDynamic(this,&UCoreAnimInstance::UCoreAnimInstance::Receiver_MovementDirection);
	

	ADelegateHelper::Transmitter_ControlRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);
	ADelegateHelper::Transmitter_CharacterRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);


	ADelegateHelper::Transmitter_JumpingStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_JumpingStatus);
	ADelegateHelper::Transmitter_CrouchStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_CrouchStatus);
	ADelegateHelper::Transmitter_InAirStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_InAirStatus);
	
}

void UCoreAnimInstance::Receiver_AnimationData(FAnimationData InAnimData)
{
}

void UCoreAnimInstance::Receiver_Velocity(FVector InValue)
{
	CurrentAnimationData.Velocity =InValue.Size();

	if(GEngine)
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Blue,*FString::Printf(TEXT(" Velocity: %f"),InValue.Size()));
	
}

void UCoreAnimInstance::Receiver_CharacterWorldLocation(FVector InValue)
{
	CurrentAnimationData.CharacterWorldLocation=InValue;
	
}

void UCoreAnimInstance::Receiver_MovementDirection(FVector InValue)
{
	CurrentAnimationData.MovementDirection=InValue;
}

void UCoreAnimInstance::Receiver_ControlRotation(FRotator InValue)
{
	CurrentAnimationData.ControllerWorldRotation =InValue;
}

void UCoreAnimInstance::Receiver_CharacterRotation(FRotator InValue)
{
	CurrentAnimationData.CharacterWorldRotation= InValue;
}

void UCoreAnimInstance::Receiver_JumpingStatus(bool InValue)
{
	CurrentAnimationData.bIsJumping=InValue;
}

void UCoreAnimInstance::Receiver_CrouchStatus(bool InValue)
{
	CurrentAnimationData.bIsCrouching=InValue;
}

void UCoreAnimInstance::Receiver_InAirStatus(bool InValue)
{
	CurrentAnimationData.bIsInAir=InValue;
}


// Add more functions and logic for other animation calculations if needed

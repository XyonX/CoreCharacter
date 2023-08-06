/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreAnimInstance.h"
#include "Proxy/CoreAnimInstanceProxy.h"
#include "CorePlugin/Helpers/DelegateHelper.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UCoreAnimInstance::UCoreAnimInstance()
: RawAnimationData() // Initializes the RawAnimationData struct with default values (e.g., 0.0f and false)

{

	//RawAnimationData=DefaultAnimationData;
	CharacterYawInterpTime = 2.0;
}

void UCoreAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	// Initialize the proxy with this anim instance
	Proxy = &Super::GetProxyOnGameThread<FCoreAnimInstanceProxy>();
	

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());

	ADelegateHelper::Transmitter_AnimationData.AddDynamic(this,&UCoreAnimInstance::Receiver_AnimationData);
	ADelegateHelper::Transmitter_Velocity.AddDynamic(this,&UCoreAnimInstance::Receiver_Velocity);
	ADelegateHelper::Transmitter_CharacterWorldLocation.AddDynamic(this,&UCoreAnimInstance::Receiver_CharacterWorldLocation);

	ADelegateHelper::Transmitter_ControlRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);
	ADelegateHelper::Transmitter_CharacterRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);


	ADelegateHelper::Transmitter_JumpingStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_JumpingStatus);
	ADelegateHelper::Transmitter_CrouchStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_CrouchStatus);
	ADelegateHelper::Transmitter_InAirStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_InAirStatus);
}

void UCoreAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	Proxy->SyncTo(this);
}

FAnimInstanceProxy* UCoreAnimInstance::CreateAnimInstanceProxy()
{
	return new FCoreAnimInstanceProxy(this);
}

void UCoreAnimInstance::DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy)
{
	delete InProxy;
}

FCoreAnimInstanceProxy* UCoreAnimInstance::GetProxyOnGameThread()
{
	// Cast the base proxy class to the custom proxy class
	return &Super::GetProxyOnGameThread<FCoreAnimInstanceProxy>();
}

void UCoreAnimInstance::Receiver_AnimationData(FCalculatedAnimationData InAnimData)
{
	
}

void UCoreAnimInstance::Receiver_Velocity(FVector InValue)
{

	RawAnimationData.Velocity =InValue;
	CalculatedAnimationData.Velocity=InValue.Size();
	

	FRotator ActorRot =RawAnimationData.CharacterRotation;
	FRotator TargetRot = FRotator(ActorRot.Pitch, RawAnimationData.ControllerRotation.Yaw, ActorRot.Roll);
	

	FRotator NewCharacterRotation = FMath::RInterpTo(ActorRot, TargetRot, GetWorld()->GetDeltaSeconds(), CharacterYawInterpTime);
	//OwnerCharacter->SetActorRotation(NewCharacterRotation);
	
	
}

void UCoreAnimInstance::Receiver_CharacterWorldLocation(FVector InValue)
{
	
	RawAnimationData.CharacterLocation=InValue;
	
}

void UCoreAnimInstance::Receiver_ControlRotation(FRotator InValue)
{
	RawAnimationData.ControllerRotation =InValue;
}

void UCoreAnimInstance::Receiver_CharacterRotation(FRotator InValue)
{
	RawAnimationData.CharacterRotation= InValue;
}

void UCoreAnimInstance::Receiver_JumpingStatus(bool InValue)
{
	RawAnimationData.bIsJumping=InValue;
}

void UCoreAnimInstance::Receiver_CrouchStatus(bool InValue)
{
	RawAnimationData.bIsCrouching=InValue;
}

void UCoreAnimInstance::Receiver_InAirStatus(bool InValue)
{
	RawAnimationData.bIsInAir=InValue;
}


// Add more functions and logic for other animation calculations if needed

/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreAnimInstanceProxy.h"
#include "CorePlugin/Data/AnimationData.h"
//#include "Kismet/GameplayStatics.h"
#include "CorePlayer/Animation/CoreAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"


FCoreAnimInstanceProxy::FCoreAnimInstanceProxy()
{
}

FCoreAnimInstanceProxy::FCoreAnimInstanceProxy(UCoreAnimInstance* InCoreAnimInstance)
:	FAnimInstanceProxy(InCoreAnimInstance),
CoreAnimInstance(InCoreAnimInstance)

{
	//DataLock = NewObject<FCriticalSection>(CoreAnimInstance->GetOwningActor());
}

FCoreAnimInstanceProxy::~FCoreAnimInstanceProxy()
{
	
}

void FCoreAnimInstanceProxy::Initialize(UAnimInstance* InAnimInstance)
{
	Super::Initialize(InAnimInstance);

	// Cast the base animation instaNce class to the custom animation instace class
	CoreAnimInstance = Cast<UCoreAnimInstance>(InAnimInstance);

	if (CoreAnimInstance)
	{
		// Sync any data from the animation instaNce to the proxy
		SyncFrom(CoreAnimInstance);
	}
}

void FCoreAnimInstanceProxy::PostEvaluate(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::PostEvaluate(InAnimInstance);
}

void FCoreAnimInstanceProxy::SyncFrom(UCoreAnimInstance* Instance)
{
	// Sync any data from the animation instace to the proxy

	// For example, copy the animation data from the animation instace to the proxy
	RawAnimationData=Instance->RawAnimationData;
}

void FCoreAnimInstanceProxy::SyncTo(UCoreAnimInstance* Instance)
{
	Instance->CalculatedAnimationData=CalculatedAnimationData;
}

void FCoreAnimInstanceProxy::CalculateAnimationData()
{
	CalculatedAnimationData.Velocity = RawAnimationData.Velocity.Size();
	
	FRotator VelocityRotation =UKismetMathLibrary::MakeRotFromX(RawAnimationData.Velocity);
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(RawAnimationData.ControllerRotation,VelocityRotation);

	
	MovementRotation = FRotator(VelocityRotation.Pitch,ControlRotation.Yaw-DeltaRotation.Yaw,VelocityRotation.Roll);

	CalculatedAnimationData.MovementRotation = MovementRotation;
}

/*
void FCoreAnimInstanceProxy::CalculateCharacterData()
{
	FRotator VelocityRotation =UKismetMathLibrary::MakeRotFromX(RawAnimationDataRef.Velocity);
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(RawAnimationDataRef.ControllerRotation,VelocityRotation);

	
	MovementRotation = FRotator(VelocityRotation.Pitch,ControlRotation.Yaw-DeltaRotation.Yaw,VelocityRotation.Roll);

	CalculatedAnimationDataRef.MovementRotation = MovementRotation;
}*/

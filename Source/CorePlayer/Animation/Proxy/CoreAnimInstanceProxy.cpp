/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreAnimInstanceProxy.h"
#include "CorePlugin/Data/AnimationData.h"
#include "Kismet/GameplayStatics.h"


FCoreAnimInstanceProxy::FCoreAnimInstanceProxy()
{
}

FCoreAnimInstanceProxy::FCoreAnimInstanceProxy(UCoreAnimInstance* InCoreAnimInstance)
:	FAnimInstanceProxy(InCoreAnimInstance),
CoreAnimInstance(InCoreAnimInstance)

{
	// Initialize default values for AnimationData
	//AnimationData.MovementSpeed = 0.0f;
	//AnimationData.bIsJumping = false;
}


bool FCoreAnimInstanceProxy::Evaluate(FPoseContext& Output)
{
	// Lock the critical section for safe data access
	//FScopeLock Lock(&CriticalSection);
	
	//Program Logic Base on the Anim Data



	// Let the base FAnimInstanceProxy handle the rest of the animation evaluation
	return FAnimInstanceProxy::Evaluate(Output);
}

void FCoreAnimInstanceProxy::UpdateAnimationData(const FAnimationData& NewAnimationData)
{
	// Update the AnimationData with the provided data from the anim instance
	AnimationData = NewAnimationData;
}

/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreAnimInstance.h"
#include "Proxy/CoreAnimInstanceProxy.h"
#include "CorePlugin/Helpers/DelegateHelper.h"

UCoreAnimInstance::UCoreAnimInstance()
: AnimationData() // Initializes the AnimationData struct with default values (e.g., 0.0f and false)

{
	DefaultAnimationData.Velocity=400;
	DefaultAnimationData.bIsJumping=false;
	
	// Initialize default values for AnimationData
	AnimationData=DefaultAnimationData;
}

void UCoreAnimInstance::NativeInitializeAnimation()
{
	
	Proxy = MakeShared<FCoreAnimInstanceProxy>(this);
	// Send a reference of the anim data to the proxy class
	Proxy->UpdateAnimationData(AnimationData);

	ADelegateHelper::Delegate_UpdateAnimationDataDelegate.AddDynamic(this,&UCoreAnimInstance::Receiver_AnimationData);
	
}

void UCoreAnimInstance::Receiver_AnimationData(FAnimationData InAnimData)
{
	
}


//Updated the anim data In the Animation instance class
void UCoreAnimInstance::UpdateAnimationData(FAnimationData InAnimationData)
{
	AnimationData=InAnimationData;
}



// Add more functions and logic for other animation calculations if needed

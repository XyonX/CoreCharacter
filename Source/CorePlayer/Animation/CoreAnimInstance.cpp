/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreAnimInstance.h"

#include "CorePlayer/Player/CorePlayerController.h"
#include "Proxy/CoreAnimInstanceProxy.h"
#include "CorePlugin/Helpers/DelegateHelper.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/ScopeTryLock.h"
#include "Threads/AnimationCalculator.h"

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
	//Proxy = &Super::GetProxyOnGameThread<FCoreAnimInstanceProxy>();

	Init();
	

	/** Creating Runnable Thread */
	
	// Create a new instance of FAnimationCalculator and pass it references to the shared raw data variable, the shared calculated data variable, and the critical section
	AnimationCalculator = new FAnimationCalculator(RawAnimationData, CalculatedAnimationData, CriticalSection,CriticalSectionCounter,this );

	// Create a new instance of FRunnableThread and pass it a pointer to the FAnimationCalculator instance and a name for the thread
	AnimationCalculatorThread = FRunnableThread::Create(AnimationCalculator, TEXT("AnimationCalculatorThread"));

	/** Binding Delegates */ 
	ADelegateHelper::Transmitter_AnimationData.AddDynamic(this,&UCoreAnimInstance::Receiver_AnimationData);
	ADelegateHelper::Transmitter_Velocity.AddDynamic(this,&UCoreAnimInstance::Receiver_Velocity);
	ADelegateHelper::Transmitter_CharacterWorldLocation.AddDynamic(this,&UCoreAnimInstance::Receiver_CharacterWorldLocation);

	ADelegateHelper::Transmitter_ControlRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);
	ADelegateHelper::Transmitter_CharacterRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);


	ADelegateHelper::Transmitter_JumpingStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_JumpingStatus);
	ADelegateHelper::Transmitter_CrouchStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_CrouchStatus);
	ADelegateHelper::Transmitter_InAirStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_InAirStatus);
}
/*
void UCoreAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//Proxy->SyncTo(this);
}*/

void UCoreAnimInstance::NativeUninitializeAnimation()
{
	// Call the base class function first
	Super::NativeUninitializeAnimation();

	// Stop and delete the thread if it exists
	if (AnimationCalculatorThread)
	{
		AnimationCalculatorThread->Kill(true);
		delete AnimationCalculatorThread;
		AnimationCalculatorThread = nullptr;
	}

	// Delete the FAnimationCalculator instance if it exists
	if (AnimationCalculator)
	{
		delete AnimationCalculator;
		AnimationCalculator = nullptr;
	}
}

void UCoreAnimInstance::Init()
{
	OwnerCharacter = Cast<ACoreCharacter>(GetOwningActor());
	OwnerController=Cast<ACorePlayerController>(UGameplayStatics::GetPlayerController(this,0));
	if(OwnerCharacter ==nullptr)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Owner IS Not Found Anim instance Init Failed "));
		}
		return;
	}
	if(OwnerController ==nullptr)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Controller IS Not Found Anim instance Init Failed "));
		}
		return;
	}
	MaxMovementSpeed=OwnerController->GetLocomotionComponent()->MaxMovementSpeed;
}

/*
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
*/
void UCoreAnimInstance::Receiver_AnimationData(FCalculatedAnimationData InAnimData)
{
	
}

void UCoreAnimInstance::Receiver_Velocity(FVector InValue)
{
	CriticalSection.Lock();
		RawAnimationData.Velocity =InValue;
	CriticalSection.Unlock();

	// Set the new data flag to true
	AnimationCalculator->VelocityChangeEvent->Trigger();

	Transmitter_AnimationData.Broadcast(GetData());

	//FRotator ActorRot =RawAnimationData.CharacterRotation;
	/*
	if(OwnerCharacter)
	{
		if(CharacterTurnCurve)
		{
			float Delta = CharacterTurnCurve->GetFloatValue(CalculatedAnimationData.Delta_Movement_Controller.Yaw);
			FRotator TargetRot = FRotator(ActorRot.Pitch, RawAnimationData.ControllerRotation.Yaw+Delta, ActorRot.Roll);
		}
		else
		{

			FRotator TargetRot = FRotator(ActorRot.Pitch, RawAnimationData.ControllerRotation.Yaw, ActorRot.Roll);
	

			FRotator NewCharacterRotation = FMath::RInterpTo(ActorRot, TargetRot, GetWorld()->GetDeltaSeconds(), CharacterYawInterpTime);
			OwnerCharacter->SetActorRotation(NewCharacterRotation);
		}
	}*/
	
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

void UCoreAnimInstance::Receiver_OnMovementStop()
{
	// Set the new data flag to true
	AnimationCalculator->NewDataAvailable = false;
}

FCalculatedAnimationData UCoreAnimInstance::GetData()
{

	if(CriticalSectionCounter.GetValue() == 0)
	{
		// Lock acquired, access the data
		return CalculatedAnimationData;
	}
	else
	{

		return AnimationCalculator->LastFrameData;
	}
}


// Add more functions and logic for other animation calculations if needed

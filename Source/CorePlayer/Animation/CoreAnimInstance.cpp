/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreAnimInstance.h"

#include "CorePlayer/Player/CorePlayerController.h"
#include "Proxy/CoreAnimInstanceProxy.h"
#include "CorePlugin/Helpers/DelegateHelper.h"
#include "DataManager/AnimationDataManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/ScopeTryLock.h"
#include "Threads/AnimationCalculator.h"


UCoreAnimInstance::UCoreAnimInstance()
: RawAnimationData() // Initializes the RawAnimationData struct with default values (e.g., 0.0f and false)

{

	//RawAnimationData=DefaultAnimationData;
	CharacterYawInterpTime = 5.0;
	


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

	AnimationDataManager=NewObject<UAnimationDataManager>(this);
	AnimationDataManager->Init(this,OwnerCharacter,OwnerController);

	/** Binding Delegates */
	/*
	ADelegateHelper::Transmitter_AnimationData.AddDynamic(this,&UCoreAnimInstance::Receiver_AnimationData);
	ADelegateHelper::Transmitter_Velocity.AddDynamic(this,&UCoreAnimInstance::Receiver_Velocity);
	ADelegateHelper::Transmitter_CharacterWorldLocation.AddDynamic(this,&UCoreAnimInstance::Receiver_CharacterWorldLocation);

	ADelegateHelper::Transmitter_ControlRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);
	ADelegateHelper::Transmitter_CharacterRotation.AddDynamic(this,&UCoreAnimInstance::Receiver_ControlRotation);


	ADelegateHelper::Transmitter_JumpingStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_JumpingStatus);
	ADelegateHelper::Transmitter_CrouchStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_CrouchStatus);
	ADelegateHelper::Transmitter_InAirStatus.AddDynamic(this,&UCoreAnimInstance::Receiver_InAirStatus);*/
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

void UCoreAnimInstance::GamethreadDataCalculator_Velocity()
{
	
}


// Add more functions and logic for other animation calculations if needed
//GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString::Printf(TEXT("The Delta yaw is : %f"),Controller_Characetr_Delta.Yaw));
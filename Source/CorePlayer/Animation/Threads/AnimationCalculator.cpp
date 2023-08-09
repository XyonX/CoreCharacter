// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationCalculator.h"

#include "CorePlayer/Animation/CoreAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

bool FAnimationCalculator::Init()

{
	// Create an event with manual reset mode
	Event_OnVelocityChange = FGenericPlatformProcess::GetSynchEventFromPool(true);
	
	MaxMovementSpeed=AnimInstanceRef->MaxMovementSpeed;
	
	return true;
}
uint32 FAnimationCalculator::Run()
{
	// This is the main loop of the thread
	while (true)
	{
		// Check if the thread should be stopped
		if (StopSignal == true)
		{
			break;
		}

		// WaitFor The velocity event call
		Event_OnVelocityChange->Wait();

		// Increment the counter before locking the critical section
		CriticalSectionCounter->Increment();

		// Lock the critical section before accessing the shared variables
		FScopeLock ScopeLock(CriticalSection);
		
		Calculate_VelocityData();

		LastFrameData = *SharedCalculatedData;

		// Reset the event after performing the calculation
		Event_OnVelocityChange->Reset();
		
		// Sleep for 10 milliseconds without generating stats data
		FPlatformProcess::SleepNoStats(0.01f);
		
	}

	// Return 0 when the thread exits
	return 0;
}

void FAnimationCalculator::Stop()
{
	// Set the stop flag to 1 to signal the thread to exit
	//StopTaskCounter.Increment();
	StopSignal=true;
	// Signal the event to wake up the thread
	Event_OnVelocityChange->Trigger();
}

void FAnimationCalculator::Calculate_VelocityData()
{
	
	Velocity = SharedRawData-> Velocity.Size();
	SharedCalculatedData->Velocity=Velocity;
	CurrentMovementStatus=EMovementStatus::Idle;

	
	// Check if the velocity is not zero to avoid division by zero errors
	if (!FMath::IsNearlyZero(Velocity))
	{
		FRotator VelocityRotation =UKismetMathLibrary::MakeRotFromX(SharedRawData->Velocity);
		SharedCalculatedData->VelocityRotation= VelocityRotation;
		SharedCalculatedData->DeltaR_MovCtlr = UKismetMathLibrary::NormalizedDeltaRotator(VelocityRotation,SharedRawData->ControllerRotation);
		SharedCalculatedData->DeltaR_CharCtlr = UKismetMathLibrary::NormalizedDeltaRotator (SharedRawData->CharacterRotation,SharedRawData->ControllerRotation);
		CalculateMovementStatus();
		CalculateMovementDirection();
	}
}

void FAnimationCalculator::CalculateMovementStatus()
{
	if(Velocity<=MaxMovementSpeed.WalkSpeed)
	{
		CurrentMovementStatus=EMovementStatus::Walking;
	}
	else if(Velocity<=MaxMovementSpeed.JogSpeed)
	{
		CurrentMovementStatus=EMovementStatus::Jogging;
	}
	else if(Velocity<=MaxMovementSpeed.SprintSpeed)
	{
		CurrentMovementStatus=EMovementStatus::Sprinting;
	}
	SharedCalculatedData->MovementStatus=CurrentMovementStatus;
}

void FAnimationCalculator::CalculateMovementDirection()
{
	float DeltaYaw = SharedCalculatedData->DeltaR_MovCtlr.Yaw;

	if (-20 < DeltaYaw && DeltaYaw < 20)
	{
		SharedCalculatedData->MovementDirection = EMovementDirection::Forward;
	}
	else if (-110 < DeltaYaw && DeltaYaw < -70)
	{
		SharedCalculatedData->MovementDirection = EMovementDirection::Left;
	}
	else if (70 < DeltaYaw && DeltaYaw < 90)
	{
		SharedCalculatedData->MovementDirection = EMovementDirection::Right;
	}
	else if (DeltaYaw < -160 || DeltaYaw > 160)
	{
		SharedCalculatedData->MovementDirection = EMovementDirection::Backward;
	}
}


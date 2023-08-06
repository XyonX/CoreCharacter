// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationCalculator.h"

#include "Kismet/KismetMathLibrary.h"

bool FAnimationCalculator::Init()

{
	// Do any initialization work here
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

		// Check if there is new data available
		if (NewDataAvailable == true)
		{
			// Lock the critical section before accessing the shared variables
			CriticalSection->Lock();
			Calculate();
			// Unlock the critical section after accessing the shared variables
			CriticalSection->Unlock();

			// Reset the new data flag
			NewDataAvailable = false;
		}
		
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
}

void FAnimationCalculator::Calculate()
{
	
		SharedCalculatedData->Velocity = SharedRawData-> Velocity.Size();
	// Check if the velocity is not zero to avoid division by zero errors
	if (!FMath::IsNearlyZero(SharedCalculatedData->Velocity))
	{
		FRotator VelocityRotation =UKismetMathLibrary::MakeRotFromX(SharedRawData->Velocity);
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(SharedRawData->ControllerRotation,VelocityRotation);
    
		SharedCalculatedData->MovementRotation =FRotator(VelocityRotation.Pitch,SharedRawData->ControllerRotation.Yaw-DeltaRotation.Yaw,VelocityRotation.Roll);
	}
    	

}

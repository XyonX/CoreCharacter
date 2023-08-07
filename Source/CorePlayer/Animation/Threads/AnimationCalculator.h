// MyMeshGenerationWorker.h
#pragma once

#include "CoreMinimal.h"
#include "CorePlugin/Data/AnimationData.h"


class UCoreAnimInstance;

// Define a class that implements FRunnable
class FAnimationCalculator : public FRunnable
{
public:
    
    // Constructor
    FAnimationCalculator(FRawAnimationData& InSharedRawData, FCalculatedAnimationData& InSharedCalculatedData, FCriticalSection& InCriticalSection ,FThreadSafeCounter&InCriticalSectionCounter, UCoreAnimInstance*CoreAnimInstance)
    {
        // Initialize the shared raw data reference, the shared calculated data reference, and the critical section reference
        SharedRawData = &InSharedRawData;
        SharedCalculatedData = &InSharedCalculatedData;
        CriticalSection = &InCriticalSection;
        CriticalSectionCounter= &InCriticalSectionCounter;
        AnimInstanceRef=CoreAnimInstance;
    }
    ~FAnimationCalculator()
    {    // Return the event to the pool
        FGenericPlatformProcess::ReturnSynchEventToPool(VelocityChangeEvent);
        VelocityChangeEvent = nullptr; }

    // Override the Init function
    virtual bool Init() override;
    // Override the Run function
    virtual uint32 Run() override;
    // Override the Stop function
    virtual void Stop() override;


    void Calculate ();

    UCoreAnimInstance*AnimInstanceRef;
    
    // A pointer to a shared raw data variable that is updated by the UAnimInstance on the game thread and accessed by this thread
    FRawAnimationData* SharedRawData;

    // A pointer to a shared calculated data variable that is updated by this thread and accessed by the FAnimInstanceProxy on a separate thread
    FCalculatedAnimationData* SharedCalculatedData;

    FCalculatedAnimationData LastFrameData;

    // A pointer to a critical section that is used to synchronize access to the shared variables
    FCriticalSection* CriticalSection;

    // Declare a thread safe counter object
    FThreadSafeCounter* CriticalSectionCounter;



    // A flag that is used to stop the thread gracefully
    //FThreadSafeCounter StopTaskCounter;
    FThreadSafeBool StopSignal;

    // Declare an atomic flag to indicate if there is new data available
    FThreadSafeBool NewDataAvailable;
    
    // Declare an event to indicate if there is new data available
    FEvent* VelocityChangeEvent;
    

private:
    //local vars
    float Velocity;
    FMovementSpeed MaxMovementSpeed ;
    FVector CurrentVelocity;
    FRotator CurrentMovementRotaion;
    EMovementStatus CurrentMovementStatus;
    EMovementDirection CurrentMovementDirection;
};
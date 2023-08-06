#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "CorePlugin/Data/AnimationData.h"
#include "CoreAnimInstanceProxy.generated.h"

class UCoreAnimInstance;

struct FCalculatedAnimationData;
class UThirdPersonAnimInstance;

// Create a custom AnimInstanceProxy struct
USTRUCT()
struct COREPLAYER_API FCoreAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	/** Constructor */
	FCoreAnimInstanceProxy();
	
	FCoreAnimInstanceProxy(UCoreAnimInstance* InCoreAnimInstance);
	~FCoreAnimInstanceProxy();


	//Overrides
	
	// Override the Initialize function to copy any data from the AnimInstance that you need for animation calculations
	//virtual void Initialize(UAnimInstance* InAnimInstance) override;

	// Override the PostEvaluate function to copy any data back to the game thread that you need for animation post-processing
	//virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;
	
	//Sync
	
	// Sync any data from the anim instance to the proxy
	//void SyncFrom(UCoreAnimInstance* Instance);

	// Sync any data from the proxy to the anim instance
	//void SyncTo(UCoreAnimInstance* Instance);


	//Calculation Function
	//void CalculateAnimationData ();
	
protected:
	
	/** Animation data that can be accessed from the rendering thread */
	FCalculatedAnimationData CalculatedAnimationData;
	FRawAnimationData RawAnimationData;

	//CODE
	FRotator MovementRotation ;
	FRotator ControlRotation;
	FRotator CharacterRotation;
	


	/** Reference to the owning UMyAnimInstance */
	UPROPERTY()
	UCoreAnimInstance* CoreAnimInstance;
	
	//FThreadSafeBool bDataUpdated;

	//FCriticalSection* DataLock;
	// Use a scoped lock for synchronization
	//FScopedLock DataLock;
	
	/** Critical section for synchronization */
	//FCriticalSection CriticalSection;
	//FThreadSafeBool bIsUpdating;
	/** Critical section for synchronization */
	//mutable FCriticalSection CriticalSection;
};



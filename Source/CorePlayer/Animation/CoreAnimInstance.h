/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CorePlugin/Data/AnimationData.h"
#include "CoreAnimInstance.generated.h"


class FAnimationCalculator;
struct FCoreAnimInstanceProxy;
UCLASS()
class COREPLAYER_API UCoreAnimInstance : public UAnimInstance
{
	
	GENERATED_BODY()

public:
	UCoreAnimInstance();
	// Override the native functions
	virtual void NativeInitializeAnimation() override;
	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUninitializeAnimation() override;

	// Override the proxy functions
	//virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	
	//virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;

	// Get the proxy on the game thread
	//FCoreAnimInstanceProxy* GetProxyOnGameThread();

	
	/** Animation data Receiver */
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_AnimationData (FCalculatedAnimationData InAnimData);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_Velocity (FVector InValue);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_CharacterWorldLocation (FVector InValue);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_ControlRotation (FRotator InValue);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_CharacterRotation (FRotator InValue);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_JumpingStatus (bool InValue);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_CrouchStatus (bool InValue);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Receiver_InAirStatus (bool InValue);

	
protected:
	

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
	FRawAnimationData RawAnimationData;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
	FCalculatedAnimationData CalculatedAnimationData;

	// A critical section that is used to synchronize access to the shared variables
	FCriticalSection CriticalSection;

	// A pointer to an instance of FAnimationCalculator that runs on a custom thread
	FAnimationCalculator* AnimationCalculator;

	// A pointer to an instance of FRunnableThread that manages the custom thread
	FRunnableThread* AnimationCalculatorThread;


protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimInstance")
	float CharacterYawInterpTime;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	float MovementOffset;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	float MovementOffsetAccel ;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	float Character_Yaw;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	float Character_Yaw_LastFrame;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	float Root_Yaw_Offset_PerFrame;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	bool bIsAccelerating ;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	float Root_Yaw_Offset;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimInstance")
	float PivotTime;
	
	float NormalTolerance ;

private:


	/** References */
	
	UPROPERTY()
	ACharacter*OwnerCharacter;
	
	// A pointer to the custom proxy class
	FCoreAnimInstanceProxy* Proxy;
	
	
};

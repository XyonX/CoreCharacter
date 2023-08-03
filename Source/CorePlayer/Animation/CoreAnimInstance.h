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


struct FCoreAnimInstanceProxy;
UCLASS()
class COREPLAYER_API UCoreAnimInstance : public UAnimInstance
{
	
	GENERATED_BODY()

public:
	UCoreAnimInstance();

	//Function only called at the starting
	virtual void NativeInitializeAnimation() override;


	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimationData(FAnimationData InAnimationData){CurrentAnimationData=InAnimationData;};

	
	/** Animation data Receiver */
	void Receiver_AnimationData (FAnimationData InAnimData);

	void Receiver_Velocity (FVector InValue);
	void Receiver_CharacterWorldLocation (FVector InValue);
	void Receiver_MovementDirection (FVector InValue);
	
	void Receiver_ControlRotation (FRotator InValue);
	void Receiver_CharacterRotation (FRotator InValue);

	void Receiver_JumpingStatus (bool InValue);
	void Receiver_CrouchStatus (bool InValue);
	void Receiver_InAirStatus (bool InValue);


	
protected:

	/** Default Animation data to start with */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FAnimationData DefaultAnimationData;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
	FAnimationData CurrentAnimationData;
	

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
	


	TSharedPtr<FCoreAnimInstanceProxy>Proxy;
	
};

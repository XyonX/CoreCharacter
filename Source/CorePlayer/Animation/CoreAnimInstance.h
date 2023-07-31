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

	//Ignore this function to Increase performance
	// Called every frame
	//virtual void NativeUpdateAnimation(float DeltaTime) override;
	void Receiver_AnimationData (FAnimationData InAnimData);

	// Function to update animation data
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimationData(FAnimationData InAnimationData);
	
protected:

	/** Default Animation data to start with */
	FAnimationData DefaultAnimationData;
	
	// Animation data that can be accessed from Blueprint
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	FAnimationData AnimationData;

	TSharedPtr<FCoreAnimInstanceProxy>Proxy;
	
};

/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "CorePlugin/Data/AnimationData.h"
#include "CorePlugin/Data/CharacterMovementData.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "LocomotionComponent.generated.h"

class ACoreCharacter;
class ACorePlayerController;


UCLASS(ClassGroup=LocomotionComponent, BlueprintType,meta=(BlueprintSpawnableComponent))
class COREPLAYER_API ULocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	ULocomotionComponent();
	virtual void BeginPlay() override;
	void Run();

//protected:
public:
	
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite,Category="Reference")
	ACharacter* OwnerCharacter;
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite,Category="Reference")
	APlayerController*OwnerController;
	UPROPERTY()
	UCharacterMovementComponent*OwnerCharacterMovementComponent;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite,Category="Defaults")
	FCharacterMovementData DefaultMovementData;
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite,Category="Defaults")
	FCharacterMovementData CharacterMovementData;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input Actions")
	class UInputAction* MovementAction ;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input Actions")
	class UInputAction* LookingAction ;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input Actions")
	class UInputAction* SprintAction; 
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input Actions")
	class UInputAction* JumpAction ;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input Actions")
	class UInputAction* FireAction ;

	// Context THAT CONTAINS INPUT ACTIONS CAN HAVE MORE THAN ONE 
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Input mapping Context")
	class UInputMappingContext*BaseMappingContext;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Defaults")
	int32 BaseMappingPriority = 0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimInstance")
	FMovementSpeed MaxMovementSpeed;

	
public:
	//Access Owner and Setup data
	void GetOwnerReference ();
	void SetCharacterDefaultSpeed ();
	void ConfigureInputAction();
	void ConfigureMappingContext();
	void ConfigureInputActionWithKey();

	// LEGACY BINDING FUNCTIONS
	UFUNCTION(BlueprintCallable ,Category="Movement",meta=(DeprecatedFunction))
	void MoveForward (float Value );
	UFUNCTION(BlueprintCallable )
	void MoveRight (float Value) ;
	UFUNCTION(BlueprintCallable ,Category="Movement")
	void Turn (float Value) ;
	UFUNCTION(BlueprintCallable ,Category="Movement")
	void LookUp(float Value);

	
	//Setting up Enhanced Subsystem

	bool BindFunctionWithInputAction(UInputComponent* PlayerInputComponent);
	bool BindContextWithSubsystem();

	// Enhanced Bindings
	UFUNCTION()
	void EnhancedMove (const FInputActionValue & Value);
	UFUNCTION()
	void OnMovementButtonReleased (const FInputActionValue & Value);
	UFUNCTION()
	void EnhancedLook (const FInputActionValue & Value);
	UFUNCTION()
	void OnSprintPressed (const FInputActionValue & Value);
	UFUNCTION()
	void OnSprintReleased (const FInputActionValue & Value);


	

	

	

	
	
	
		
};

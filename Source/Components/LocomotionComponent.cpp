/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/

#include "LocomotionComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/CoreCharacterEXTENDED.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values for this component's properties
ULocomotionComponent::ULocomotionComponent()
{
	
	CharacterMovementData.TurnRate=1.5;
	CharacterMovementData.LookUpRate=1.5;
}


// Called when the game starts
void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwnerReference();
	BindContextWithSubsystem();
	
}
void ULocomotionComponent::GetOwnerReference()
{
	
	if(Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter=Cast<ACharacter>(GetOwner());
		OwnerController=UGameplayStatics::GetPlayerController(GetWorld(),0);
	}
	else if (Cast<APlayerController>(GetOwner()))
	{
		OwnerController=Cast<APlayerController>(GetOwner());
		OwnerCharacter=Cast<ACharacter>(OwnerController->GetPawn());
	}
}

void ULocomotionComponent::MoveForward(float Value)
{
	if(OwnerCharacter && OwnerController ==nullptr)
	{
		return	;
	}

	if(Value!=0)
	{
		// getting controller rotation and finding the forward vector from control rotation
		FRotator ControlRotation = OwnerController->GetControlRotation();
		FRotator YawRot(0.0f, ControlRotation.Yaw, 0.f);
		FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		UE_LOG(LogTemp, Warning, TEXT("Moving Without Freelook") );
		OwnerCharacter->AddMovementInput(ForwardDirection,Value);
	}

	
}

void ULocomotionComponent::MoveRight(float Value)
{
	if(OwnerCharacter && OwnerController ==nullptr)
	{
		return	;
	}
	
	if(Value!=0)
	{
		// getting controller rotation and finding the forward vector from control rotation
		FRotator ControlRotation = OwnerController->GetControlRotation();
		FRotator YawRot(0.0f, ControlRotation.Yaw, 0.f);
		FVector RightDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		UE_LOG(LogTemp, Warning, TEXT("Moving Without Freelook") );
		OwnerCharacter->AddMovementInput(RightDirection,Value);
	}
			
	
			
}

void ULocomotionComponent::Turn(float Value)
{
	if(OwnerCharacter && OwnerController ==nullptr)
	{
		return	;
	}
	OwnerController->AddYawInput(Value*CharacterMovementData.TurnRate);

	
}

void ULocomotionComponent::LookUp(float Value)
{
	if(OwnerCharacter && OwnerController ==nullptr)
	{
		return	;
	}
	OwnerController->AddPitchInput(Value*CharacterMovementData.LookUpRate);

}

void ULocomotionComponent::EnhancedMove(const FInputActionValue& Value)
{
	if(OwnerCharacter && OwnerController ==nullptr)
	{
		return	;
	}
	if(Value.GetMagnitude() != 0)

	{
		UPawnMovementComponent*MovementComponent = OwnerCharacter->GetMovementComponent();

		FRotator CtrlRotation =  OwnerController->GetControlRotation();
		FRotator YawRot(0.0f, CtrlRotation.Yaw, 0.f);
	
		FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);


		MovementComponent->AddInputVector(ForwardDirection*Value[0]);
		MovementComponent->AddInputVector(RightDirection*Value[1]);
	}


}

void ULocomotionComponent::EnhancedLook(const FInputActionValue& Value)
{
	if(OwnerCharacter && OwnerController ==nullptr)
	{
		return	;
	}
	OwnerCharacter->AddControllerYawInput(Value[0]*CharacterMovementData.TurnRate);
	OwnerCharacter->AddControllerPitchInput(Value[1]*CharacterMovementData.LookUpRate);
	UE_LOG(LogTemp, Warning, TEXT("Enhanced Looking Function Callewd") );
	
}

bool ULocomotionComponent::BindContextWithSubsystem()
{
	if(OwnerController == nullptr)
	{
		return false; ;
	}
	if(UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwnerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(BaseMappingContext,BaseMappingPriority);

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Mapping Context Setup Sucessfull "));
		}
		
		return true;
	}
	else
	{
		return false;
	}
	

	
}

bool ULocomotionComponent::BindActionWithInputSystem(UInputComponent* PlayerInputComponent)
{
	if(OwnerCharacter == nullptr)
	{
		return false; ;
	}
	
	UEnhancedInputComponent*EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EIC)
	{
		EIC->BindAction(MovementAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::EnhancedMove);
		EIC	->BindAction(LookingAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::EnhancedLook );
		UE_LOG(LogTemp, Warning, TEXT("Movement Action Binding Done FROM SETUP INPUT") );
		
		return true;
	}
	else
	{
		return false;
	}

}





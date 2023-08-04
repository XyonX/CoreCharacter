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
#include "CorePlayer/Character/CoreCharacterEXTENDED.h"
#include "CorePlugin/Helpers/DelegateHelper.h"
#include "CorePlugin/Helpers/InputHandlerHelper.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values for this component's properties
ULocomotionComponent::ULocomotionComponent()
{
	
	DefaultMovementData.TurnRate=1.5;
	DefaultMovementData.LookUpRate=1.5;
	CharacterMovementData=DefaultMovementData;
	BaseMappingPriority=0;
}


// Called when the game starts
void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwnerReference();
	BindContextWithSubsystem();
	BindFunctionWithInputAction(OwnerController->InputComponent);

	
}

void ULocomotionComponent::Run()
{
	GetOwnerReference();
	ConfigureInputAction();
	ConfigureMappingContext();
	ConfigureInputActionWithKey();
	BindContextWithSubsystem();
}

void ULocomotionComponent::GetOwnerReference()
{
	
	if(Cast<ACharacter>(GetOwner()))
	{
		OwnerPawn=Cast<ACharacter>(GetOwner());
		OwnerController=UGameplayStatics::GetPlayerController(GetWorld(),0);
	}

	if (Cast<APlayerController>(GetOwner()))
	{
		OwnerController=Cast<APlayerController>(GetOwner());
		OwnerPawn=OwnerController->GetPawn();
		
	}
}

void ULocomotionComponent::ConfigureInputAction()
{
	MovementAction= UInputHandlerHelper::CreateInputAction(OwnerPawn,EInputActionValueType::Axis2D);
	LookingAction=UInputHandlerHelper::CreateInputAction(OwnerPawn,EInputActionValueType::Axis2D);
}

void ULocomotionComponent::ConfigureMappingContext()
{
	BaseMappingContext=UInputHandlerHelper::CreateInputMappingContext(OwnerPawn);
}

void ULocomotionComponent::ConfigureInputActionWithKey()
{

	UInputHandlerHelper::MapKeyWithInputAction(BaseMappingContext, MovementAction, EKeys::W);
	UInputHandlerHelper::MapKeyWithInputAction(BaseMappingContext, MovementAction, EKeys::S, true);
	UInputHandlerHelper::MapKeyWithInputAction(BaseMappingContext, MovementAction, EKeys::A, true, true);
	UInputHandlerHelper::MapKeyWithInputAction(BaseMappingContext, MovementAction, EKeys::D, false, true);

	UInputHandlerHelper::MapKeyWithInputAction(BaseMappingContext,LookingAction,EKeys::MouseX);
	UInputHandlerHelper::MapKeyWithInputAction(BaseMappingContext,LookingAction,EKeys::MouseY,false,true);
}

void ULocomotionComponent::MoveForward(float Value)
{
	if(OwnerPawn && OwnerController ==nullptr)
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
		OwnerPawn->AddMovementInput(ForwardDirection,Value);
	}

	
}

void ULocomotionComponent::MoveRight(float Value)
{
	if(OwnerPawn && OwnerController ==nullptr)
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
		OwnerPawn->AddMovementInput(RightDirection,Value);
	}
			
	
			
}

void ULocomotionComponent::Turn(float Value)
{
	if(OwnerPawn && OwnerController ==nullptr)
	{
		return	;
	}
	OwnerController->AddYawInput(Value*CharacterMovementData.TurnRate);

	
}

void ULocomotionComponent::LookUp(float Value)
{
	if(OwnerPawn && OwnerController ==nullptr)
	{
		return	;
	}
	OwnerController->AddPitchInput(Value*CharacterMovementData.LookUpRate);

}

void ULocomotionComponent::EnhancedMove(const FInputActionValue& Value)
{
	
	if(OwnerPawn && OwnerController ==nullptr)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No valid character or controller found   "));
		}
		return	;
	}
	if(Value.GetMagnitude() != 0)

	{
		UPawnMovementComponent*MovementComponent = OwnerPawn->GetMovementComponent();

		FRotator CtrlRotation =  OwnerController->GetControlRotation();
		FRotator YawRot(0.0f, CtrlRotation.Yaw, 0.f);
	
		FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);


		MovementComponent->AddInputVector(ForwardDirection*Value[0]);
		MovementComponent->AddInputVector(RightDirection*Value[1]);

		FVector Velocity  = OwnerPawn->GetMovementComponent()->Velocity;
		
		ADelegateHelper::Transmitter_Velocity.Broadcast(Velocity);
		ADelegateHelper::Transmitter_CharacterRotation.Broadcast(OwnerPawn->GetActorRotation());
		ADelegateHelper::Transmitter_ControlRotation.Broadcast(OwnerController->GetControlRotation());
		
	}


}

void ULocomotionComponent::EnhancedLook(const FInputActionValue& Value)
{

	if(OwnerPawn && OwnerController ==nullptr)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No valid character or controller found   "));
		}
		return	;
	}
	OwnerPawn->AddControllerYawInput(Value[0]*CharacterMovementData.TurnRate);
	OwnerPawn->AddControllerPitchInput(Value[1]*CharacterMovementData.LookUpRate);
	
}

bool ULocomotionComponent::BindContextWithSubsystem()
{
	if(OwnerController == nullptr)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Context  binding with subsystem failed , no controller found  "));
		}
		return false	;
	}
	if(UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwnerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(BaseMappingContext,BaseMappingPriority);
		
		return true;
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("  context binding failed ,Enable Enhanced input sytem from the project setting  "));
		}
		return false;
	}
	

	
}

bool ULocomotionComponent::BindFunctionWithInputAction(UInputComponent* PlayerInputComponent)
{
	if(OwnerPawn == nullptr)
	{
		
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OwnerPawn NOT FOUND "));
		}
		return false;
			
	}
	
	UEnhancedInputComponent*EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if(EIC ==nullptr)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("  BindFunction failed with action ,Enable Enhanced input sytem from the project setting  "));
		}
		return false;
	}
	
	EIC->BindAction(MovementAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::EnhancedMove);
	EIC	->BindAction(LookingAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::EnhancedLook );
	
	return true;
}





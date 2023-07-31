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
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green,TEXT("Component fucntion called"));
}

void ULocomotionComponent::GetOwnerReference()
{
	
	if(Cast<ACharacter>(GetOwner()))
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Locomotion component owned by character class "));
		}
		OwnerPawn=Cast<ACharacter>(GetOwner());
		OwnerController=UGameplayStatics::GetPlayerController(GetWorld(),0);
	}

	if (Cast<APlayerController>(GetOwner()))
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Locomotion component owned by controller class "));
		}
		OwnerController=Cast<APlayerController>(GetOwner());
		OwnerPawn=OwnerController->GetPawn();
		
		if(OwnerPawn)
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CONTROLLER PAWN FOUND   "));
			}
			
		}
		
		
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
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enhanced move called "));
	}
	if(OwnerPawn && OwnerController ==nullptr)
	{
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
	}


}

void ULocomotionComponent::EnhancedLook(const FInputActionValue& Value)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enhanced Lookup Called "));
	}
	if(OwnerPawn && OwnerController ==nullptr)
	{
		return	;
	}
	OwnerPawn->AddControllerYawInput(Value[0]*CharacterMovementData.TurnRate);
	OwnerPawn->AddControllerPitchInput(Value[1]*CharacterMovementData.LookUpRate);
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
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Eic cast failed "));
		}
		return false;
	}
	
	EIC->BindAction(MovementAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::EnhancedMove);
	EIC	->BindAction(LookingAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::EnhancedLook );
	

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Key Binding Sucessfull "));
	}
	return true;
}





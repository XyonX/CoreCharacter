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
#include "CorePlayer/Animation/DataManager/AnimationDataManager.h"
#include "CorePlayer/Character/CoreCharacterEXTENDED.h"
#include "CorePlugin/Helpers/DelegateHelper.h"
#include "CorePlugin/Helpers/InputHandlerHelper.h"
#include "CorePlugin/Helpers/TimerHandlerHelper.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULocomotionComponent::ULocomotionComponent()
{
	
	DefaultMovementData.TurnRate=1.5;
	DefaultMovementData.LookUpRate=1.5;
	CharacterMovementData=DefaultMovementData;
	BaseMappingPriority=0;

	MaxMovementSpeed.WalkSpeed=200;
	MaxMovementSpeed.JogSpeed=400;
	MaxMovementSpeed.SprintSpeed=600;
	bIsMoving=false;
	bIsRotating=false;
}


// Called when the game starts
void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwnerReference();
	SetCharacterDefaultSpeed();
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
		OwnerCharacter=Cast<ACharacter>(GetOwner());
		OwnerController=UGameplayStatics::GetPlayerController(GetWorld(),0);
	}

	if (Cast<APlayerController>(GetOwner()))
	{
		OwnerController=Cast<APlayerController>(GetOwner());
		OwnerCharacter=Cast<ACharacter>(OwnerController->GetPawn());
		
	}
}

void ULocomotionComponent::SetCharacterDefaultSpeed()
{
	if(OwnerCharacter)
	{
		OwnerCharacterMovementComponent= OwnerCharacter->GetCharacterMovement();
		OwnerCharacterMovementComponent->MaxWalkSpeed=MaxMovementSpeed.JogSpeed;
	}
}

void ULocomotionComponent::ConfigureInputAction()
{
	MovementAction= UInputHandlerHelper::CreateInputAction(OwnerCharacter,EInputActionValueType::Axis2D);
	LookingAction=UInputHandlerHelper::CreateInputAction(OwnerCharacter,EInputActionValueType::Axis2D);
}

void ULocomotionComponent::ConfigureMappingContext()
{
	BaseMappingContext=UInputHandlerHelper::CreateInputMappingContext(OwnerCharacter);
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
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No valid character or controller found   "));
		}
		return	;
	}

	UPawnMovementComponent*MovementComponent = OwnerCharacter->GetMovementComponent();

	FRotator ControlRotation = OwnerController->GetControlRotation();
	
	FRotator YawRot(0.0f, ControlRotation.Yaw, 0.f);

	FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);


	MovementComponent->AddInputVector(ForwardDirection*Value[0]);
	MovementComponent->AddInputVector(RightDirection*Value[1]);
	

	if(!bIsMoving)
	{
		bIsMoving=true;
		
		float TimerRate = 1.0f / 60.0f; // Interval for 60 FPS (approx. 0.0167 seconds)
		bool bLoop = true; // Set to true if you want the timer to loop
		float FirstDelay = 0.0f; // Initial delay before the first timer call (optional)

		GetWorld()->GetTimerManager().SetTimer(ATimerHandlerHelper::OnMovementTimer,ADelegateHelper::OnMovement, 0.016, bLoop, FirstDelay);
	}


}

void ULocomotionComponent::OnMovementButtonReleased(const FInputActionValue& Value)
{
	GetWorld()->GetTimerManager().ClearTimer(ATimerHandlerHelper::OnMovementTimer); 
}

void ULocomotionComponent::EnhancedLook(const FInputActionValue& Value)
{

	if(OwnerCharacter && OwnerController ==nullptr)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No valid character or controller found   "));
		}
		return	;
	}
	OwnerCharacter->AddControllerYawInput(Value[0]*CharacterMovementData.TurnRate);
	OwnerCharacter->AddControllerPitchInput(Value[1]*CharacterMovementData.LookUpRate);

	if(!bIsRotating)
	{
		bIsRotating=true;
		
		float TimerRate = 1.0f / 60.0f; // Interval for 60 FPS (approx. 0.0167 seconds)
		bool bLoop = true; // Set to true if you want the timer to loop
		float FirstDelay = 0.0f; // Initial delay before the first timer call (optional)

		GetWorld()->GetTimerManager().SetTimer(ATimerHandlerHelper::OnRotationTimer,ADelegateHelper::OnMovement, 0.016, bLoop, FirstDelay);
	}
	
}

void ULocomotionComponent::OnSprintPressed(const FInputActionValue& Value)
{
	OwnerCharacterMovementComponent->MaxWalkSpeed=MaxMovementSpeed.SprintSpeed;
}

void ULocomotionComponent::OnSprintReleased(const FInputActionValue& Value)
{
	OwnerCharacterMovementComponent->MaxWalkSpeed=MaxMovementSpeed.JogSpeed;
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
	if(OwnerCharacter == nullptr)
	{
		
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OwnerCharacter NOT FOUND "));
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
	EIC->BindAction(MovementAction,ETriggerEvent::Completed,this,&ULocomotionComponent::OnMovementButtonReleased);
	EIC->BindAction(LookingAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::EnhancedLook );
	EIC->BindAction(SprintAction,ETriggerEvent::Triggered,this,&ULocomotionComponent::OnSprintPressed);
	EIC->BindAction(SprintAction,ETriggerEvent::Completed,this,&ULocomotionComponent::OnSprintReleased);
	
	return true;
}





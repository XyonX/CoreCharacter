/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CorePlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include"InputModifiers.h"
#include "GameFramework/Character.h"

/** Map key to action for mapping context with optional modifiers. */

static void MapKey(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate = false, bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* Outer = InputMappingContext->GetOuter();

	if (bNegate)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}

	if (bSwizzle)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

ACorePlayerController::ACorePlayerController()
{
	LocomotionComponent=CreateDefaultSubobject<ULocomotionComponent>("Locomotion Component");
}

void ACorePlayerController::BeginPlay()
{
	Super::BeginPlay();
	LocomotionComponent->BindContextWithSubsystem();
}

void ACorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	ControlledCharacter=Cast<ACharacter>(GetPawn());
	LocomotionComponent->BindActionWithInputSystem(InputComponent);

	
	/*
	// creating the oinput mapping object context
	BaseMappingContext = NewObject <UInputMappingContext>(this);

	// creating the input  action
	MovementAction = NewObject<UInputAction>(this);

	
	MovementAction->ValueType = EInputActionValueType::Axis3D;

	// WITHOUT HELPER FUNCTION
	CoreMappingContext->MapKey(MoveAction,EKeys::W);
	FEnhancedActionKeyMapping&Mapping = CoreMappingContext->MapKey(MoveAction,EKeys::S);
	UInputModifier*Negate = NewObject <UInputModifierNegate>(this);
	Mapping.Modifiers.Add(Negate);

	MapKey(BaseMappingContext, MovementAction, EKeys::W);
	MapKey(BaseMappingContext, MovementAction, EKeys::S, true);
	MapKey(BaseMappingContext, MovementAction, EKeys::A, true, true);
	MapKey(BaseMappingContext, MovementAction, EKeys::D, false, true);

	*/
}

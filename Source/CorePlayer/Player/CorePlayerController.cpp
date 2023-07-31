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

ACorePlayerController::ACorePlayerController()
{
	LocomotionComponent=CreateDefaultSubobject<ULocomotionComponent>("Locomotion Component");
}

void ACorePlayerController::BeginPlay()
{
	Super::BeginPlay();
	//LocomotionComponent->BindContextWithSubsystem();
	//LocomotionComponent->GetOwnerReference();
	//LocomotionComponent->ConfigureInputAction();
	//LocomotionComponent->ConfigureMappingContext();
	//LocomotionComponent->BindContextWithSubsystem();
	

}

void ACorePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	//LocomotionComponent->GetOwnerReference();
	//LocomotionComponent->BindContextWithSubsystem();
	//LocomotionComponent->BindFunctionWithInputAction(InputComponent);
}


void ACorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}


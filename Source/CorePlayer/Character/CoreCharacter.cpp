/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreCharacter.h"
#include "CorePlayer/Components/CoreCameraComponent.h"
#include "CorePlayer/Components/CorePawnComponentExtended.h"
#include"GameFramework/SpringArmComponent.h"
#include "CorePlayer/Components/CoreHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "CorePlayer/Player/CorePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CorePlayer/Components/LocomotionComponent.h"

ACoreCharacter::ACoreCharacter()
{

	/* Sprint Arm*/
	SpringArm =CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true ;

	/* Sprint Arm for ADS */
	SpringArmADS = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring ArmADS"));
	SpringArmADS->SetupAttachment(RootComponent);
	SpringArmADS->bUsePawnControlRotation = true ;

	/* Camera */
	CameraComponent = CreateDefaultSubobject<UCoreCameraComponent>(TEXT("Camera Component ")) ;
	check(CameraComponent)
	CameraComponent->SetupAttachment(SpringArm);

	/* Health Component */
	HealthComponent =CreateDefaultSubobject<UCoreHealthComponent>(TEXT("Health Component "));

	

	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;

	UCapsuleComponent*CapsuleComp = GetCapsuleComponent();
	CapsuleComp->InitCapsuleSize(40.0,90.0f);
	

	GetCharacterMovement()->bOrientRotationToMovement=true;

	
}

void ACoreCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ACoreCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

/** Getters */

ACorePlayerController* ACoreCharacter::GetCorePlayerController() const
{
	if(Controller)
	{
		ACorePlayerController * PlayerController = CastChecked<ACorePlayerController>(Controller);
		if(PlayerController)
		{
			return PlayerController;
		}
	}
	return nullptr;
	
}


/** Ability System Events */

void ACoreCharacter::OnAbilitySystemInitialized()
{
}

void ACoreCharacter::OnAbilitySystemUninitialized()
{
}

/** Player lifecycle management */

void ACoreCharacter::OnDeathStarted(AActor* OwningActor)
{
}

void ACoreCharacter::OnDeathFinished(AActor* OwningActor)
{
}

void ACoreCharacter::DisableMovementAndCollision()
{
}

void ACoreCharacter::DestroyDueToDeath()
{
}

void ACoreCharacter::UnInitAndDestroy()
{
}

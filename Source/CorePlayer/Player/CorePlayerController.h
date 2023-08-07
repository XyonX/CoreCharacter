/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "CorePlayer/Components/LocomotionComponent.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COREPLAYER_API ACorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACorePlayerController();
	// for input bindings
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	ULocomotionComponent* GetLocomotionComponent (){return LocomotionComponent;};
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	ULocomotionComponent*LocomotionComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Reference")
	ACharacter*ControlledCharacter;
	
	
};

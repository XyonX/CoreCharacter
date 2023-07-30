/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include"CorePlugin/Data/PlayerData.h"
#include "CoreCharacter.generated.h"

/* Forward Declaration */
class ACorePlayerController;
class UCoreAbilitySystemComponent;
class UCorePawnComponentExtended;
class UCoreHealthComponent ;
class UCoreCameraComponent ;
class ULocomotionComponent;
class USpringArmComponent;

UCLASS()
class COREPLAYER_API ACoreCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ACoreCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	
	/** Getters */
	UFUNCTION(BlueprintCallable,Category="CoreCharacter")
	ACorePlayerController* GetCorePlayerController()const;
	UFUNCTION(BlueprintCallable,Category="Getters")
	FORCEINLINE UCoreCameraComponent*GetCameraComponent(){return CameraComponent; }

	
	/* Player Events */

	/** Ability System Events */
	// when ability system component is available then we we want to call this function
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	/** Lifecycle Management */
	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);
	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UnInitAndDestroy();
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))	//Blueprint implementation of Death finish Event
	void K2_OnDeathFinished();


	
protected:


	/** Camera Components */
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Components")
	UCoreCameraComponent*CameraComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Components")
	USpringArmComponent*SpringArm ;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Components")
	USpringArmComponent*SpringArmADS;
	


	/** Custom Components */
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Components")
	UCoreHealthComponent*HealthComponent;

   
	
	
};

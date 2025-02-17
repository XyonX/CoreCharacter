/******************************************************************************
* Project Core - Generic UE Project
* Copyright (c) [2023] [ Joydip chakraborty ]
* This class is part of the ProjectCore open-source project. 
* ******************************************************************************/


#include "CoreHealthComponent.h"
#include "CoreAbilitySystemComponent.h"

void UCoreHealthComponent::InitializeWithAbilitySystem(UCoreAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner ();
	check(Owner);
	if(AbilitySystemComponent)
	{
		
		UE_LOG(LogTemp, Error, TEXT("LyraHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}
	AbilitySystemComponent = InASC ;
	if(!AbilitySystemComponent)
	{

		UE_LOG(LogTemp,Error,TEXT("Owner : %s  Doesnt Have An Active Ability System Comp "),*GetNameSafe(Owner));
	}
	//HealthSet = UCoreAbilitySystemComponent->GetSet
}

void UCoreHealthComponent::UninitializeFromAbilitySystem()
{
	AbilitySystemComponent =	nullptr ;
}

float UCoreHealthComponent::GetHealth() const
{
	return 69;
}

float UCoreHealthComponent::GetMaxHealth() const
{
	return 69 ;
}

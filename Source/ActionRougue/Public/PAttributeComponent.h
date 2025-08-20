// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PAttributeComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor,UPAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UPAttributeComponent*, OwningComp, float, NewRage, float, Delta);

//Alternative : share the same signature with Generic names
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributesChanged, AActor*, InstigatorActor, UPAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGUE_API UPAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPAttributeComponent();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	static UPAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable,Category="Attributes",meta=(DisplayName="IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated, Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated, Category="Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated, Category="Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Replicated, Category="Attributes")
	float RageMax;	

	UFUNCTION(NetMulticast, Reliable) //@fixme: mark as unreliable once we move the 'state' our of PHeroCharacter
	void MultiCastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable) //Used for cosmetic change only
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsFullHealth() const;
	
	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyHealthChange (AActor* InstigatorActor , float Delta);

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributesChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnAttributesChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetRage() const;

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyRage(AActor* InstigatorActor, float Delta); 

		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PTargetDummy.h"

// Sets default values
APTargetDummy::APTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	SetRootComponent(CubeMesh);

	AttributeComp = CreateDefaultSubobject<UPAttributeComponent>(TEXT("AttributeComp"));
	AttributeComp->OnHealthChanged.AddDynamic(this,&APTargetDummy::OnHealthChanged);

}


void APTargetDummy::OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		CubeMesh->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
	}
}




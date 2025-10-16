// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "ComponentRotator.h"
#include "ToolBuilderUtil.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Check if ActorMove isn't null
	if (ActorWithMovableComp)
	{
		FindCompMovers();
	}

	// Check if is a Pressure Plate
	if (IsPressurePlate)
	{
		// Bind the delegate OnOverlapBegin
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		// Bind the delegate OnOverlapEnd
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Function to set Trigger state
void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	// Set IsTriggered with the new value passed by parameter
	IsTriggered = NewTriggerValue;

	for (auto CompMover : CompMovers)
	{
		CompMover->SetShouldMove(IsTriggered);
	}

	for (auto CompRotator : CompRotators)
	{
		CompRotator->SetShouldRotate(IsTriggered);
	}
}

void UTriggerComponent::FindCompMovers()
{

	TArray<USceneComponent*> Components;
	ActorWithMovableComp->GetComponents<USceneComponent>(Components);
	
	for (USceneComponent* Component : Components)
	{
		if (Component->ComponentHasTag("CompMover"))
		{
			CompMovers.Add(Cast <UComponentMover>(Component));
		}

		if (Component->ComponentHasTag("CompRotator"))
		{
			CompRotators.Add(Cast <UComponentRotator>(Component));
		}
	}

	if (CompMovers.IsEmpty() && CompRotators.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Movable Component not found in the Actor: %s"), *ActorWithMovableComp->GetActorNameOrLabel());
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if Other actor is valid and if has the tag "PressurePlateActivator"
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		// Increment the count of the actors in the trigger
		ActiveOverlaps++;

		// Check if the Trigger isn't active
		if (!IsTriggered)
		{
			// Active the Trigger
			Trigger(true);
		}
	}
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if Other actor is valid and if has the tag "PressurePlateActivator"
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		// Decrement the count of the actors in the trigger
		ActiveOverlaps--;

		// Check if the Trigger is active and doesn't has actor inside of him
		if (IsTriggered && ActiveOverlaps == 0 && !OneWay)
		{
			// Deactivate the Trigger
			Trigger(false);
		}
	}
}

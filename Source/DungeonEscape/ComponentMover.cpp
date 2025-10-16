// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentMover.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UComponentMover::UComponentMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add("CompMover");
}


// Called when the game starts
void UComponentMover::BeginPlay()
{
	Super::BeginPlay();

	// Call a function that finds the MovableComp and check if is Valid
	FindMovableComp();

	// Set the StartLocation with a relative location of the component
	StartLocation = MovableComp->GetRelativeLocation();

	SetShouldMove(false);
}


// Called every frame
void UComponentMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Call a function that finds the MovableComp and check if is Valid
	FindMovableComp();

	FVector CurrentLocation = MovableComp->GetRelativeLocation();

	if (!CurrentLocation.Equals(TargetLocation))
	{
		// Calculate the owner movement speed based in the Offset and the Time. (Eg: 200cm/4s = 50cm/s)
		float Speed = MoveOffset.Length() / MoveTime;

		/* Set the NewLocation Vector with Interpolated value between CurrentLocation and TargetLocation based in the
		Speed and DeltaTime */
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		
		// Apply the NewLocation to Owner
		MovableComp->SetRelativeLocation(NewLocation);
	}
}

void UComponentMover::FindMovableComp()
{
	// Set MovableComp with the parent of this component
	MovableComp = GetAttachParent();

	// Check if MovableComp is null and if the Owner Actor exists
	if (!MovableComp && GetOwner())
	{
		// Set MovableComp with the found component by "MovableComp" tag in the self-Actor
		MovableComp = GetOwner()->FindComponentByTag<USceneComponent>("MovableComp");
	}

	// Check if MovableComp is null and show an Error Message
	if (!MovableComp)
	{
		UE_LOG(LogTemp, Error, TEXT("MovableComp not found in the Actor: %s"), *GetOwner()->GetActorNameOrLabel());
	}
}

// Get the current state of ShouldMove
bool UComponentMover::GetShouldMove() const
{
	return ShouldMove;
}

void UComponentMover::SetShouldMove(bool NewShouldMove)
{
	// Update the ShouldMove state only if the value changed
	if (ShouldMove != NewShouldMove)
	{
		ShouldMove = NewShouldMove;
	}

	// Check if ShouldMove is True
	if (ShouldMove)
	{
		// Set TargetLocation as the StartLocation + the desired offset
		TargetLocation = StartLocation + MoveOffset;
	}
	else
	{
		// Set TargetLocation as the StartLocation
		TargetLocation = StartLocation;
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentRotator.h"

#include "Fonts/UnicodeBlockRange.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UComponentRotator::UComponentRotator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add("CompRotator");
}


// Called when the game starts
void UComponentRotator::BeginPlay()
{
	Super::BeginPlay();

	// Call a function that finds the MovableComp and check if is Valid
	FindMovableComp();

	// Set the StartRotation with relative rotation of the component
	StartRotation = MovableComp->GetRelativeRotation();

	// Initialize ShouldRotate as false
	SetShouldRotate(false);

}


// Called every frame
void UComponentRotator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Call a function that finds the MovableComp and check if is Valid
	FindMovableComp();

	// Get the current rotation of MovableComponent
	FRotator CurrentRotation = MovableComp->GetRelativeRotation();

	// Check if the current rotation isn't equal to the target rotation
	if (!CurrentRotation.Equals(TargetRotation))
	{
		/* Define the RotationSpeed in Degrees based on the AngleDistance (Distance between Start and Target)
		and RotationTime. Eg. 90°/4s = 22.5°/s */
		float RotationSpeed = FMath::RadiansToDegrees(AngleDistance) / RotationTime;

		// Interpolates the CurrentRotation with TargetRotation with a fixed Step per second
		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

		// Apply the NewRotation to the component
		MovableComp->SetRelativeRotation(NewRotation);
	}

	SetShouldRotate(ShouldRotate);
}


void UComponentRotator::FindMovableComp()
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

// Get the current state of ShouldRotate
bool UComponentRotator::GetShouldRotate() const
{
	return ShouldRotate;
}

// Set the new state of ShouldRotate based on parameter
void UComponentRotator::SetShouldRotate(bool NewShouldRotate)
{
	// Update the ShouldRotate state only if the value changed
	if (ShouldRotate != NewShouldRotate)
	{
		ShouldRotate = NewShouldRotate;
	}

	// Check if ShouldRotate is True
	if (ShouldRotate)
	{
		RotationTime = RotationTimeActive;
		
		// Set TargetRotation as the StartRotation + the desired rotation
		TargetRotation = StartRotation + RotationAngle;

		// Calculate the AngleDistance between StartRotation and TargetRotation
		AngleDistance = StartRotation.Quaternion().AngularDistance(TargetRotation.Quaternion());
	}
	else
	{
		RotationTime = RotationTimeDeactive;
		
		// Set TargetRotation as the StartRotation
		TargetRotation = StartRotation;
	}
}

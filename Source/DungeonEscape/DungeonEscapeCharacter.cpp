// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonEscapeCharacter.h"

#include "CollectableItem.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DungeonEscape.h"
#include "LockItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProfilingDebugging/CookStats.h"

ADungeonEscapeCharacter::ADungeonEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADungeonEscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeonEscapeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADungeonEscapeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::LookInput);

		// Interact
		EnhancedInputComponent->BindAction(InterectAction, ETriggerEvent::Started, this, &ADungeonEscapeCharacter::Interact);
	}
	else
	{
		UE_LOG(LogDungeonEscape, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ADungeonEscapeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ADungeonEscapeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ADungeonEscapeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeonEscapeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADungeonEscapeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ADungeonEscapeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void ADungeonEscapeCharacter::Interact()
{
	// Set the Start of Raycast with camera location value
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	// Set the End of Raycast based in InteractionDistance forward of camera
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * InteractionDistance);

	// Draw a red debug line based in the Start and End Location
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);

	// Create a SphereShape
	FCollisionShape InteractionShape = FCollisionShape::MakeSphere(InteractSphereRadius);
	
	// Draw a green debug sphere based in the End of de Raycast line and the Radius
	DrawDebugSphere(GetWorld(), End, InteractSphereRadius, 10, FColor::Green, false, 1.0f, 0, 1.0f);

	// Declare a variable to store the last Hit Infos
	FHitResult HitResult;

	/* SweepSingleByChannel: Do a trace of a single actor Sweeping a shape from Start to End location, ignoring
	rotation and only for actors in the specified channel (In this case is a custom: ECC_GameTraceChannel2 = Interact),
	this also update the HitResult with last hit infos.*/
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, InteractionShape);

	// Do something similar to the Sweep, but can draw the trace shape
	/*
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, InteractSphereRadius,
	UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false,
	TArray<AActor*>(), EDrawDebugTrace::ForDuration,HitResult, true);
	*/
	
	// Check if the Trace hit something
	if (bHit)
	{
		// Store the actor that received the trace hit
		AActor* HitActor = HitResult.GetActor();

		// Check if the Actor has Tag "CollectableItem"
		if (HitActor->ActorHasTag(TEXT("CollectableItem")))
		{
			// Try to convert the HitActor to the CollectableItem class with a Safe Cast and check if CollectableItem
			// isn't null (successfully cast)
			if (ACollectableItem* CollectableItem = Cast<ACollectableItem>(HitActor))
			{
				// Add this Item to the Inventory array (FString)
				Inventory.Add(CollectableItem->ItemName);
				// Destroy the Item Actor
				CollectableItem->Destroy();
			}
		}
		// Check if the Actor has Tag "LockItem"
		else if (HitActor->ActorHasTag(TEXT("LockItem")))
		{
			// Try to convert the HitActor to the LockItem class with a Safe Cast and check if LockItem
			// isn't null (successfully cast)
			if (ALockItem* LockItem = Cast<ALockItem>(HitActor))
			{

				// Check if LockItem don't have a Key
				if (!LockItem->GetIsKeyPlaced())
				{
					// Remove the item from the inventory and store the value (1 = 1 element removed, 0 = not found)
					/* RemoveSingle: check if the array contains the element and remove the first occurence in the list,
					also return a int value (0 = not found | 1 = Removed 1 item) */
					int32 ItemsRemoved = Inventory.RemoveSingle(LockItem->KeyItemName);

					// Check if the item was removed, if so, set the IsKeyPlaced to true
					if (ItemsRemoved == 1)
					{
						LockItem->SetIsKeyPlaced(true);
					}
				}
				// Check if LockItem already have a Key, the take it back
				else
				{
					// IsKeyPlaced to false
					LockItem->SetIsKeyPlaced(false);
					// Add this Item to the Inventory array (FString)
					Inventory.Add(LockItem->KeyItemName);
				}
			}
		}
	}
}

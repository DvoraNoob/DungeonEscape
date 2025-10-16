// Fill out your copyright notice in the Description page of Project Settings.


#include "LockItem.h"

// Sets default values
ALockItem::ALockItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the RootComponent of the Actor
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	// Set the RootComp as the RootComponent
	SetRootComponent(RootComp);

	// Create the Trigger Component (This is the Trigger of the actor)
	TriggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("Trigger Comp"));
	// Attach the TriggerComp to the RootComp
	TriggerComp->SetupAttachment(RootComp);

	// Create the KeyItemMesh Component (This is the mesh of the actor)
	KeyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyItemMesh"));
	// Attach the KeyItemMesh to the RootComp
	KeyItemMesh->SetupAttachment(RootComp);

	// Add a Actor Tag "LockItem" for the Actor
	Tags.Add("LockItem");
}

// Called when the game starts or when spawned
void ALockItem::BeginPlay()
{
	Super::BeginPlay();

	// Set IsKeyPlaced as false (the LockItem/Stand doesn't have the Key Object)
	SetIsKeyPlaced(IsKeyPlaced);
}

// Called every frame
void ALockItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Getter, Return the current value of IsKeyPlaced
bool ALockItem::GetIsKeyPlaced() const
{
	return IsKeyPlaced;
}

// Setter, Define the state of LockItem (Has or not the Key)
void ALockItem::SetIsKeyPlaced(bool NewIsKeyPlaced)
{
	// Set IsKeyPlaced with the parameter value
	IsKeyPlaced = NewIsKeyPlaced;

	// Set the Trigger State with parameter value (True = Active, False = Deactivated)
	TriggerComp->Trigger(NewIsKeyPlaced);
	//Set the KeyItemMesh visibility (True = Visible, False = Not Visible)
	KeyItemMesh->SetVisibility(NewIsKeyPlaced);
}


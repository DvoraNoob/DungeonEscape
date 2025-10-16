// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TriggerComponent.h"
#include "Components/StaticMeshComponent.h"

#include "LockItem.generated.h"

UCLASS()
class DUNGEONESCAPE_API ALockItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Default|Lock Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default|Lock Components")
	UTriggerComponent* TriggerComp;

	UPROPERTY(VisibleAnywhere, Category = "Default|Lock Components")
	UStaticMeshComponent* KeyItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta=(ExposeOnSpawn=true))
	FString KeyItemName;

	// Getter of IsKeyPlaced 
	UFUNCTION()
	bool GetIsKeyPlaced() const;

	// Setter of IsKeyPlaced
	UFUNCTION()
	void SetIsKeyPlaced(bool NewIsKeyPlaced);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta=(ExposeOnSpawn=true))
	bool IsKeyPlaced = false;
};

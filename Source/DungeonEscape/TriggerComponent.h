// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentMover.h"
#include "ComponentRotator.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UTriggerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Trigger Settings", meta=(ExposeOnSpawn="true"))
	bool IsPressurePlate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Trigger Settings", meta=(ExposeOnSpawn="true"))
	bool OneWay = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Trigger Settings", meta=(ExposeOnSpawn="true"))
	AActor* ActorWithMovableComp;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Component|Trigger Settings")
	TArray<UComponentMover*> CompMovers;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Component|Trigger Settings")
	TArray<UComponentRotator*> CompRotators;
	

	UPROPERTY(VisibleAnywhere, Category = "Trigger Component|Trigger Infos")
	bool IsTriggered = false;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Component|Trigger Infos")
	int32 ActiveOverlaps = 0;

	UFUNCTION()
	void Trigger (bool NewTriggerValue);

	UFUNCTION()
	void FindCompMovers();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

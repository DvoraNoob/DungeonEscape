// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ComponentRotator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UComponentRotator : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentRotator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FRotator RotationAngle;

	UPROPERTY(EditAnywhere)
	float RotationTimeActive;
	
	UPROPERTY(EditAnywhere);
	float RotationTimeDeactive;

	float RotationTime;

	UPROPERTY()
	USceneComponent* MovableComp;

	UPROPERTY()
	FRotator StartRotation;
	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	float AngleDistance;

	UFUNCTION()
	void FindMovableComp();

	UFUNCTION()
	bool GetShouldRotate() const;

	UFUNCTION()
	void SetShouldRotate(bool NewShouldRotate);

private:
	UPROPERTY(EditAnywhere)
	bool ShouldRotate = false;
};

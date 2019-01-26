// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SquirrelMovement.generated.h"

/**
 * 
 */
UCLASS()
class DOGHOME_API USquirrelMovement : public UPawnMovementComponent
{
	GENERATED_BODY()

private:
	FRotator CurrentRotation;
	FVector MoveVector;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
		
public:
	void Move(float DeltaTime, FVector Destination);
	void RotateTowards(float DeltaTime, FVector Destination);
};

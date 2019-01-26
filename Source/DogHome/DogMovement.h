// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "DogMovement.generated.h"

UCLASS()
class DOGHOME_API UDogMovement : public UPawnMovementComponent
{
	GENERATED_BODY()

private:

	class ADog* Dog = nullptr;
	class USkeletalMeshComponent* Mesh = nullptr;

	FVector InputVector;
	FVector Move;
	int32 JustJumped;
	FRotator CurrentRotation;


protected:
	bool UseGravity = true;
	float ZVel;

public:
	virtual void BeginPlay() override;	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
		virtual bool IsGrounded();

	void Jump();

private:
	bool CheckGroundedAtPosition(FVector Position);
};

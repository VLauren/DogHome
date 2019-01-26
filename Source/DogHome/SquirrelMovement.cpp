// Fill out your copyright notice in the Description page of Project Settings.

#include "SquirrelMovement.h"
#include "Squirrel.h"

void USquirrelMovement::BeginPlay()
{
	Super::BeginPlay();

	MoveVector = FVector::ZeroVector;
}

void USquirrelMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USquirrelMovement::Move(float DeltaTime, FVector Destination)
{
	FVector direction;
	direction = Destination - GetOwner()->GetActorLocation();
	direction.Z = 0;
	direction.Normalize();

	// Move
	FVector move = direction * DeltaTime * Cast<ASquirrel>(GetOwner())->MovementSpeed;
	move.Z = 0;
	FHitResult Hit;

	MoveVector = FMath::Lerp(MoveVector, move, 0.1f); // TODO move lerp speed variable

	SafeMoveUpdatedComponent(MoveVector, UpdatedComponent->GetComponentRotation(), true, Hit);
	if (Hit.IsValidBlockingHit())
		SlideAlongSurface(move, 1.f - Hit.Time, Hit.Normal, Hit);

	if (!MoveVector.IsNearlyZero())
	{
		// Movement rotation
		FRotator ctrlRot = MoveVector.Rotation();

		CurrentRotation = FMath::Lerp(CurrentRotation, ctrlRot, 0.1f);
		UpdatedComponent->GetOwner()->SetActorRotation(CurrentRotation);
	}
}

void USquirrelMovement::RotateTowards(float DeltaTime, FVector Destination)
{
	// Movement rotation
	FRotator ctrlRot = (Destination - GetOwner()->GetActorLocation()).Rotation();
	CurrentRotation = FMath::Lerp(CurrentRotation, ctrlRot, 0.1f);
	UpdatedComponent->GetOwner()->SetActorRotation(CurrentRotation);
}


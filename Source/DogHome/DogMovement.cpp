
#include "DogMovement.h"
#include "Dog.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"

void UDogMovement::BeginPlay()
{
	Super::BeginPlay();

	Dog = (ADog*)GetOwner();

	Move = FVector::ZeroVector;
	ZVel = 0;
}

void UDogMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsGrounded() || !UseGravity)
	{
		ZVel = 0.0f;
	}
	else
	{
		ZVel -= Cast<ADog>(GetOwner())->GravityStrength * DeltaTime;
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector(0, 0, ZVel), UpdatedComponent->GetComponentRotation(), true, Hit);
	if (Hit.IsValidBlockingHit())
		SlideAlongSurface(FVector(0, 0, ZVel), 1.f - Hit.Time, Hit.Normal, Hit);

	// ==========================================================

	if (!PawnOwner || !UpdatedComponent || Dog == nullptr || ShouldSkipUpdate(DeltaTime))
		return;

	// Movement vector calculation
	FVector movementVector;
	InputVector = ConsumeInputVector().GetClampedToMaxSize(1.0f);
	movementVector = InputVector * Dog->MovementSpeed;
	movementVector *= DeltaTime;

	Move = FMath::Lerp(Move, movementVector, Dog->StopLerpSpeed);

	if (!Move.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Move, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If it hits something, it slides along its surface
		if (Hit.IsValidBlockingHit())
			SlideAlongSurface(Move, 1.f - Hit.Time, Hit.Normal, Hit);

		if (!movementVector.IsNearlyZero())
		{
			// Target rotation
			FRotator ctrlRot = movementVector.Rotation();
			// Rotate character towards target rotation
			CurrentRotation = FMath::Lerp(CurrentRotation, ctrlRot, Dog->RotationLerpSpeed);
			UpdatedComponent->GetOwner()->SetActorRotation(CurrentRotation);
		}
	}

	// Ignore gravity frames
	if (JustJumped > 0)
		JustJumped--;
}

bool UDogMovement::IsGrounded()
{
	if (JustJumped > 0)
		return false;

	float CapsuleHalfHeight = Cast<UCapsuleComponent>(UpdatedComponent)->GetUnscaledCapsuleHalfHeight();
	float Radius = Cast<UCapsuleComponent>(UpdatedComponent)->GetScaledCapsuleRadius();
	FVector Position = UpdatedComponent->GetOwner()->GetActorLocation() - FVector(0, 0, CapsuleHalfHeight - Radius + 3.0f);

	// Grounded sphere
	// DrawDebugSphere(GetWorld(), Position, Radius, 8, FColor::Green);

	return CheckGroundedAtPosition(Position);
}

bool UDogMovement::CheckGroundedAtPosition(FVector Position)
{
	float Radius = Cast<UCapsuleComponent>(UpdatedComponent)->GetScaledCapsuleRadius();

	FHitResult OutHit;
	FCollisionQueryParams ColParams;

	// Spherecast to check ground collision
	if (GetWorld()->SweepSingleByChannel(OutHit, Position, Position, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(Radius)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UDogMovement::Jump()
{
	if (Dog == nullptr)
		return;

	UseGravity = true;
	ZVel = Dog->JumpStrength;
	JustJumped = 4;
}

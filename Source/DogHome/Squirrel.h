// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Squirrel.generated.h"

UENUM()
enum class ESquirrelState : uint8
{
	START,
	CHASE_BALL,
	FOLLOW_PATH,
	WAIT_DOG,
	RUN_AWAY
};

UCLASS()
class DOGHOME_API ASquirrel : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASquirrel();

private:
	int routeIndex;
	float frameCount;
	FVector BallPosition;
	AActor* Ball;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CollisionBox;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
		class USquirrelMovement* Movement;
public:	

	static ASquirrel* Instance;

	UPROPERTY(EditAnywhere, Category = MovementValues)
		float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = MovementValues)
		float NormalMovementSpeed;
	UPROPERTY(EditAnywhere, Category = MovementValues)
		float SlowMovementSpeed;
	UPROPERTY(EditAnywhere, Category = MovementValues)
		float FastMovementSpeed;

	// State
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESquirrelState State;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void StartChaseBall(FVector position, AActor* ball);

};

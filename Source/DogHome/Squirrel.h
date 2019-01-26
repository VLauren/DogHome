// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Squirrel.generated.h"

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
	UPROPERTY(EditAnywhere, Category = MovementValues)
		float MovementSpeed;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

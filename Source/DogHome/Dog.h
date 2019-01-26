// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Dog.generated.h"

UCLASS()
class DOGHOME_API ADog : public APawn
{
	GENERATED_BODY()

private:

	bool disabledInput;

public:

	static ADog* Instance;

	UPROPERTY(EditAnywhere)
		float GravityStrength;
	UPROPERTY(EditAnywhere)
		float MovementSpeed;
	UPROPERTY(EditAnywhere)
		float RotationLerpSpeed;
	UPROPERTY(EditAnywhere)
		float JumpStrength;
	UPROPERTY(EditAnywhere)
		float StopLerpSpeed;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// ASquirrel* LockTarget;


	// Sets default values for this pawn's properties
	ADog();

protected:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDogMovement* Movement;

public:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void DisableMove();
	UFUNCTION(BlueprintCallable)
		void EnableMove();

private:
	// Input methods
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void YawInput(float Val);
	void Jump();

};

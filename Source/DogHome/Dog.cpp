#include "Dog.h"
#include "DogMovement.h"
#include "Squirrel.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Components/InputComponent.h"

ADog* ADog::Instance;

// Sets default values
ADog::ADog()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Add the character's capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(18.0f, 24.0f);
	CapsuleComponent->SetCollisionProfileName("MainChar");
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->bCheckAsyncSceneOnMove = false;
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	CapsuleComponent->SetVisibility(true);
	CapsuleComponent->SetHiddenInGame(false);

	// Camera's spring arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f;

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5;

	// Add the camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocation(FVector(0, 0, 50));

	// Input does not rotate the camera but the spring arm
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = true;

	// Mesh
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh != nullptr)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}

	// Movement component
	Movement = CreateDefaultSubobject<UDogMovement>(TEXT("Movement"));
	Movement->UpdatedComponent = RootComponent;

	// Default values
	MovementSpeed = 250.0f;
	RotationLerpSpeed = 0.1f;
	JumpStrength = 7;
	GravityStrength = 20.0f;
	StopLerpSpeed = 0.09f;

	Instance = this;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	
	StartPos = GetActorLocation();
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UE_LOG(LogTemp, Warning, TEXT("Dist squirrel:%f"), FVector::Distance(ASquirrel::Instance->GetActorLocation(), GetActorLocation()));

	// Ardilla se escapa
	if (!IsNight && !disabledInput && FVector::Distance(ASquirrel::Instance->GetActorLocation(), GetActorLocation()) > 1800)
	{
		DisableMove();
		OnSquirrelGotAway();
	}

	// UE_LOG(LogTemp, Warning, TEXT("Tick distance:%f"), FVector::Distance(ADog::Instance->GetActorLocation(), StartPos));

	// Llegar a casa de noche
	if (IsNight && FVector::Distance(StartPos, GetActorLocation()) < 500 && !End)
	{
		OnArriveHome();
		End = true;
	}
}

// Called to bind functionality to input
void ADog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Camera input
	PlayerInputComponent->BindAxis("Turn", this, &ADog::YawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Movement input
	PlayerInputComponent->BindAxis("MoveRight", this, &ADog::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ADog::MoveForward);

	// Actions input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADog::Jump);
}

void ADog::SetLockTarget(ASquirrel* target)
{
	LockTarget = target;
}

void ADog::DisableMove()
{
	disabledInput = true;
}

void ADog::EnableMove()
{
	disabledInput = false;
}

void ADog::StartNight()
{
	IsNight = true;
}

void ADog::MoveForward(float AxisValue)
{
	if (disabledInput)
		return;

	if (Movement && (Movement->UpdatedComponent == RootComponent))
		Movement->AddInputVector(FRotator(0, GetControlRotation().Yaw, 0).RotateVector(FVector(1, 0, 0)) * AxisValue);
}

void ADog::MoveRight(float AxisValue)
{
	if (disabledInput)
		return;

	if (Movement && (Movement->UpdatedComponent == RootComponent))
		Movement->AddInputVector(FRotator(0, GetControlRotation().Yaw, 0).RotateVector(FVector(0, 1, 0)) * AxisValue);

	AddControllerYawInput(AxisValue / 6);
}

void ADog::YawInput(float Val)
{
	if (LockTarget == nullptr)
	{
		AddControllerYawInput(Val);
	}
	else
	{
		FVector direction = ((LockTarget->GetActorLocation() - FVector(0,0, 100)) - GetActorLocation()).GetSafeNormal();
		Controller->SetControlRotation(direction.Rotation());
	}
}

void ADog::Jump()
{
	if (!Movement->IsGrounded() || disabledInput)
		return;

	Movement->Jump();
	OnJump();
}


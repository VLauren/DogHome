
#include "Squirrel.h"
#include "SquirrelMovement.h"
#include "HomeSquare.h"
#include "Dog.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

ASquirrel* ASquirrel::Instance;

ASquirrel::ASquirrel()
{
	PrimaryActorTick.bCanEverTick = true;

	Instance = this;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->InitBoxExtent(FVector(50, 50, 50));
	CollisionBox->SetCollisionProfileName("Paco");
	CollisionBox->CanCharacterStepUpOn = ECB_No;
	CollisionBox->bCheckAsyncSceneOnMove = false;
	CollisionBox->SetCanEverAffectNavigation(false);
	CollisionBox->bDynamicObstacle = true;
	RootComponent = CollisionBox;

	CollisionBox->SetVisibility(true);
	CollisionBox->SetHiddenInGame(false);

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		// Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CollisionBox);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetCanEverAffectNavigation(false);
	}

	Movement = CreateDefaultSubobject<USquirrelMovement>(TEXT("Movimiento"));
	Movement->UpdatedComponent = RootComponent;

	MovementSpeed = 250.0f;
	NormalMovementSpeed = 250.0f;
	FastMovementSpeed = 400.0f;
	SlowMovementSpeed = 200.0f;
}

// Called when the game starts or when spawned
void ASquirrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASquirrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// frameCount += DeltaTime * 60;
	// if (frameCount < 60)
		// return;

	if (State == ESquirrelState::START)
	{
		// Nada
	}
	else if (State == ESquirrelState::CHASE_BALL)
	{
		Movement->Move(DeltaTime, BallPosition);

		// if (FVector::Distance(GetActorLocation(), BallPosition) < 80)
		// {
			// Ball->RemoveFromRoot();
			// Ball->Destroy();

			// TArray<UStaticMeshComponent*> Meshes;
			// Ball->GetComponents<UStaticMeshComponent>(Meshes);
			// Meshes[0]->SetVisibility(false);

			// Cast<UStaticMeshComponent>(Ball->GetComponentByClass(UStaticMeshComponent::StaticClass()))->SetVisibility(false);

			// State = ESquirrelState::FOLLOW_PATH;
			// ADog::Instance->EnableMove();
		// }
	}
	else if (State == ESquirrelState::FOLLOW_PATH)
	{
		// Si es el final me paro
		if (routeIndex < AHomeSquare::SquirrelPath.Num())
		{
			// Me muevo hacia el siguiente punto
			FVector destination = AHomeSquare::GetSquareLocation(AHomeSquare::SquirrelPath[routeIndex].X, AHomeSquare::SquirrelPath[routeIndex].Y);
			Movement->Move(DeltaTime, destination);

			// Si he llegado, aumento el indice
			if (FVector::Distance(GetActorLocation(), destination) < 200)
				routeIndex++;

			// UE_LOG(LogTemp, Warning, TEXT("Tick distance:%f"), FVector::Distance(GetActorLocation(), destination))
			// UE_LOG(LogTemp, Warning, TEXT("Tick distance:%f"), FVector::Distance(ADog::Instance->GetActorLocation(), GetActorLocation()));

			float dist = FVector::Distance(ADog::Instance->GetActorLocation(), GetActorLocation());
			if (dist < 300)
				MovementSpeed = FastMovementSpeed;
			else if (dist > 400 && dist < 600)
				MovementSpeed = NormalMovementSpeed;
			else if (dist > 700)
				MovementSpeed = SlowMovementSpeed;
		}
		else
		{
			State = ESquirrelState::WAIT_DOG;
		}
	}
	else if (State == ESquirrelState::WAIT_DOG)
	{
		Movement->RotateTowards(DeltaTime, ADog::Instance->GetActorLocation());
	}
	else if (State == ESquirrelState::RUN_AWAY)
	{
		FVector destination = GetActorLocation() + (GetActorLocation() - ADog::Instance->GetActorLocation()).GetSafeNormal() * 5000;
		Movement->Move(DeltaTime, destination);
	}

}

void ASquirrel::StartChaseBall(FVector position, AActor* ball)
{
	State = ESquirrelState::CHASE_BALL;
	BallPosition = position;
	Ball = ball;

	// ADog::Instance->SetLockTarget(this);
}


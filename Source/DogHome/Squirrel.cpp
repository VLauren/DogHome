
#include "Squirrel.h"
#include "SquirrelMovement.h"
#include "HomeSquare.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

ASquirrel::ASquirrel()
{
	PrimaryActorTick.bCanEverTick = true;

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
		Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
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

	frameCount += DeltaTime * 60;
	if (frameCount < 60)
		return;

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
	}



}

// Called to bind functionality to input
void ASquirrel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


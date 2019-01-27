
#include "Square.h"
#include "Components/BoxComponent.h"
// #include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

ASquare::ASquare()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetBoxExtent(FVector(300, 300, 50));
	RootComponent = Box;
	Box->SetVisibility(false);

	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// if (Mesh != nullptr)
	// {
		// Mesh->AlwaysLoadOnClient = true;
		// Mesh->AlwaysLoadOnServer = true;
		// Mesh->bOwnerNoSee = false;
		// Mesh->bCastDynamicShadow = true;
		// Mesh->bAffectDynamicIndirectLighting = true;
		// Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		// static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		// Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		// Mesh->SetGenerateOverlapEvents(false);
		// Mesh->SetCanEverAffectNavigation(false);
		// Mesh->SetRelativeLocation(FVector::ZeroVector);
	// }
}

void ASquare::BeginPlay()
{
	Super::BeginPlay();
}

void ASquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


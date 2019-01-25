
#include "HomeSquare.h"
#include "Components/BoxComponent.h"

AHomeSquare::AHomeSquare()
{
	PrimaryActorTick.bCanEverTick = true;

	Map.Init(1, MAP_SIZE * MAP_SIZE);
}

void AHomeSquare::BeginPlay()
{
	Super::BeginPlay();

	GridPosX = MAP_SIZE / 2;
	GridPosY = MAP_SIZE / 2;

	Map[GridPosX * MAP_SIZE + GridPosY] = 0;

	// Generate Grid


	// Spawn Grid
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			FVector loc = GetSquareLocation(i, j);

			if (Map[i * MAP_SIZE + j] == 1 && A != nullptr)
			{
				FActorSpawnParameters Params;
				if (FMath::RandRange(0, 1) == 1)
					GetWorld()->SpawnActor<ASquare>(A, loc, FRotator::ZeroRotator, Params);
				else
					GetWorld()->SpawnActor<ASquare>(B, loc, FRotator::ZeroRotator, Params);
				UE_LOG(LogTemp, Warning, TEXT("Spawn A at (%d,%d) "), i, j);
			}
		}
	}

	// FActorSpawnParameters Params;
	// FVector loc = GetSquareLocation(2, 2);
	// GetWorld()->SpawnActor<ASquare>(A, loc, FRotator::ZeroRotator, Params);
}

void AHomeSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



FVector AHomeSquare::GetSquareLocation(int i, int j)
{
	float length = Box->GetScaledBoxExtent().X * 2;
	return FVector((i - GridPosX) * length, (j - GridPosX) * length, 0);
}

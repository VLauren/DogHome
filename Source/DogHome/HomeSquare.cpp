
#include "HomeSquare.h"
#include "Components/BoxComponent.h"

TArray<FIntPoint> AHomeSquare::SquirrelPath;
AHomeSquare* AHomeSquare::Instance;

AHomeSquare::AHomeSquare()
{
	PrimaryActorTick.bCanEverTick = true;

	Map.Init(2, MAP_SIZE * MAP_SIZE);
}

void AHomeSquare::StartNight()
{

}

void AHomeSquare::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;

	GridPosX = MAP_SIZE / 2;
	GridPosY = MAP_SIZE / 2;

	////////////////////////////////////////////
	// Generate Grid

	// Punto inicial
	Map[GridPosX * MAP_SIZE + GridPosY] = 0;
	UE_LOG(LogTemp, Warning, TEXT("Size: %d, HomeSquare: (%d,%d) "), MAP_SIZE, GridPosX, GridPosY);

	// Camino
	bool squirrelPathDone = false;
	SquirrelPath.Reset();

	// while (!pathDone)
	for (int i = 0; i < 5; i++)
	{
		int startX = GridPosX + 1;
		int startY = GridPosY;

		if (!squirrelPathDone)
			SquirrelPath.Add(FIntPoint(startX, startY));

		int endX, endY, dir;
		Path(startX, startY, 3, 3, endX, endY);
		dir = 1;

		// HACK
		dir = i % 3; if (dir > 2)dir -= 3; if (dir < 0) dir += 3;

		Path(startX, startY, 3, dir, endX, endY);
		startX = endX; startY = endY;

		if (!squirrelPathDone)
			SquirrelPath.Add(FIntPoint(startX, startY));

		for (int i = 0; i < 8; i++)
		{
			// Giro derecha/izquierda
			int rand = FMath::RandRange(0, 2);
			int length = FMath::RandRange(4, 7);

			if (rand == 0)
				dir++;
			else if(rand == 1)
				dir--;

			if (dir < 0) dir += 4;
			if (dir > 3) dir -= 4;

			// Siguiente recta
			Path(startX, startY, 3, dir, endX, endY);
			startX = endX; startY = endY;

			// Punto de la ruta de la ardilla
			if (!squirrelPathDone && SquirrelPath.Num() < SQUIRREL_PATH_LENGTH)
				SquirrelPath.Add(FIntPoint(startX, startY));

		}
		squirrelPathDone = true;
	}

	// Lo que no sea camino ni inicio
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (Map[i * MAP_SIZE + j] != 0 && Map[i * MAP_SIZE + j] != 1)
			{
				 if (FMath::RandRange(0, 1) == 0)
					 // Map[i * MAP_SIZE + j] = 1;
					 Map[i * MAP_SIZE + j] = 2;
				 else
					 // Map[i * MAP_SIZE + j] = 1;
					 Map[i * MAP_SIZE + j] = 3;
			}
		}
	}

	Map[GridPosX * MAP_SIZE + GridPosY] = 1;

	////////////////////////////////////////////
	// Spawn Grid

	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			FVector loc = GetSquareLocation(i, j);

			FActorSpawnParameters Params;
			int index;

			// Path
			if (Map[i * MAP_SIZE + j] == 1 && PathSquareA != nullptr)
				GetWorld()->SpawnActor<ASquare>(PathSquareA, loc, FRotator::ZeroRotator, Params);

			// Block
			index = FMath::RandRange(0, BlockSquares.Num() - 1);
			if (Map[i * MAP_SIZE + j] == 2 && BlockSquares[index] != nullptr)
				GetWorld()->SpawnActor<ASquare>(BlockSquares[index], loc, FRotator::ZeroRotator, Params);

			// Pass
			index = FMath::RandRange(0, PassSquares.Num() - 1);
			if (Map[i * MAP_SIZE + j] == 3 && PassSquares[index] != nullptr)
				GetWorld()->SpawnActor<ASquare>(PassSquares[index], loc, FRotator::ZeroRotator, Params);

			// UE_LOG(LogTemp, Warning, TEXT("Spawn A at (%d,%d) "), i, j);
			// UE_LOG(LogTemp, Warning, TEXT("Test %d"), FMath::RandRange(0, 3));
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
	float length = Instance->Box->GetScaledBoxExtent().X * 2;
	return FVector((i - Instance->GridPosX) * length, (j - Instance->GridPosX) * length, 0);
}

void AHomeSquare::Path(int startX, int startY, int length, int direction, int & endX, int & endY)
{
	if (direction == 0)
	{
		for (int i = 0; i < length; i++)
		{
			if ((startX + i) * MAP_SIZE + startY < Map.Num())
				Map[(startX + i) * MAP_SIZE + startY] = 1;
		}

		endX = startX + length;
		endY = startY;
	}
	else if (direction == 1)
	{
		for (int i = 0; i < length; i++)
		{
			if (startX * MAP_SIZE + startY + i < Map.Num())
				Map[startX * MAP_SIZE + startY + i] = 1;
		}

		endX = startX;
		endY = startY + length;
	}
	else if (direction == 2)
	{
		for (int i = 0; i < length; i++)
		{
			if ((startX - i) * MAP_SIZE + startY < Map.Num())
				Map[(startX - i) * MAP_SIZE + startY] = 1;
		}

		endX = startX - length;
		endY = startY;
	}
	else if (direction == 3)
	{
		for (int i = 0; i < length; i++)
		{
			if (startX * MAP_SIZE + startY - i < Map.Num())
				Map[startX * MAP_SIZE + startY - i] = 1;
		}

		endX = startX;
		endY = startY - length;
	}
}

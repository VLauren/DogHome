
#pragma once

// https://www.fmod.com/docs/api/content/generated/engine_ue4/getting_started.html
// https://docs.unrealengine.com/en-us/Engine/Sequencer/HowTo/TracksFade


#include "CoreMinimal.h"
#include "Square.h"
#include "HomeSquare.generated.h"

static const int MAP_SIZE = 50;
static const int SQUIRREL_PATH_LENGTH = 3;

UCLASS()
class DOGHOME_API AHomeSquare : public ASquare
{
	GENERATED_BODY()
	
public:
	static AHomeSquare* Instance;
	static TArray<FIntPoint> SquirrelPath;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASquare> PathSquareA;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class ASquare>> BlockSquares;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class ASquare>> PassSquares;

	TArray<int> Map;

	AHomeSquare();

	UFUNCTION(BlueprintCallable)
		void StartNight();

protected:
	virtual void BeginPlay() override;

	void Path(int startX, int startY, int length, int direction, int& endX, int& endY);

public:	
	static FVector GetSquareLocation(int i, int j);

	virtual void Tick(float DeltaTime) override;

};

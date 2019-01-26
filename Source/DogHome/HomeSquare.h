
#pragma once

#include "CoreMinimal.h"
#include "Square.h"
#include "HomeSquare.generated.h"

static const int MAP_SIZE = 50;

UCLASS()
class DOGHOME_API AHomeSquare : public ASquare
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASquare> PathSquareA;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASquare> A;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASquare> B;

	TArray<int> Map;

	AHomeSquare();

protected:
	virtual void BeginPlay() override;

	FVector GetSquareLocation(int i, int j);

	void Path(int startX, int startY, int length, int direction, int& endX, int& endY);

public:	
	virtual void Tick(float DeltaTime) override;

};

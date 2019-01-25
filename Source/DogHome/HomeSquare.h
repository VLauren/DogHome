
#pragma once

#include "CoreMinimal.h"
#include "Square.h"
#include "HomeSquare.generated.h"

static const int MAP_SIZE = 30;

UCLASS()
class DOGHOME_API AHomeSquare : public ASquare
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASquare> A;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASquare> B;

	TArray<int> Map;

	AHomeSquare();

protected:
	virtual void BeginPlay() override;

	FVector GetSquareLocation(int i, int j);

public:	
	virtual void Tick(float DeltaTime) override;

};

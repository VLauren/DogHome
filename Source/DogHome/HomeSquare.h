
#pragma once

#include "CoreMinimal.h"
#include "Square.h"
#include "HomeSquare.generated.h"

/**
 * 
 */
UCLASS()
class DOGHOME_API AHomeSquare : public ASquare
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
		ASquare* A;
	UPROPERTY(VisibleAnywhere)
		ASquare* B;

	AHomeSquare();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

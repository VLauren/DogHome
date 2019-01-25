
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// #include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Square.generated.h"

UCLASS()
class DOGHOME_API ASquare : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box;

public:	
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		// class UStaticMeshComponent* Mesh;

	int GridPosX;
	int GridPosY;

	ASquare();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

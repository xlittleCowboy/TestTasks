#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingPawn.generated.h"

class UFloatingPawnMovement;

UCLASS()
class TESTTASKS_API AFlyingPawn : public APawn
{
	GENERATED_BODY()

	UFUNCTION()
	void OnPathPointsFound(const TArray<FVector>& Points);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UFloatingPawnMovement* FloatingPawnMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn")
	bool bFlyToTargetOnBeginPlay = true;

	/** If TargetActor is not set, then pawn will fly to the TargetLocation. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Flying Pawn")
	AActor* TargetActor = nullptr;

	/** If TargetActor is not set, then pawn will fly to the TargetLocation. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Flying Pawn", meta=(EditCondition="TargetActor == nullptr", EditConditionHides="true"))
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn")
	float RotationAlpha = 0.1f;

	virtual void BeginPlay() override;

public:
	AFlyingPawn();

	virtual void Tick(float DeltaSeconds) override;
};

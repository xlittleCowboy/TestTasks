#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingPawn.generated.h"

class UFloatingPawnMovement;

UCLASS()
class TESTTASKS_API AFlyingPawn : public APawn
{
	GENERATED_BODY()

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn|Debug")
	bool bDrawDebugPath = false;

	/** Grid size for path building. Should be > 10.\n\n 
	 * FOR DEBUG PATH DRAWING ONLY! Actual value should be set in the FlyTo node in the behaviour tree. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn|Debug", meta=(ClampMin=11.0f, EditCondition="bDrawDebugPath", EditConditionHides="true"))
	float PathGridSize = 100.0f;

	/** FOR DEBUG PATH DRAWING ONLY! Actual value should be set in the FlyTo node in the behaviour tree. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn|Debug", meta=(EditCondition="bDrawDebugPath", EditConditionHides="true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleObjectTypes;

	void DrawDebugPath(const TArray<FVector>& Path) const;

	virtual void BeginPlay() override;

public:
	AFlyingPawn();
	
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;
};

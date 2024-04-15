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
	bool bFlyToTargetActorOnBeginPlay = true;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Flying Pawn")
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn|Debug")
	bool bDrawDebugPath = false;

	/** Grid size for path building. Should be > 10.\n\n 
	 * FOR DEBUG PATH DRAWING ONLY! Actual value should be set in the FlyTo node in the behaviour tree. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn|Debug", meta=(ClampMin=11.0f, EditCondition="bDrawDebugPath", EditConditionHides="true"))
	float PathGridSize = 50.0f;

	/** FOR DEBUG PATH DRAWING ONLY! Actual value should be set in the FlyTo node in the behaviour tree. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn|Debug", meta=(EditCondition="bDrawDebugPath", EditConditionHides="true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleObjectTypes;

	void DrawDebugPath(const TArray<FVector>& Path) const;

	virtual void BeginPlay() override;

public:
	AFlyingPawn();
	
	virtual void OnConstruction(const FTransform& Transform) override;
};

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FlyTo.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKS_API UBTTask_FlyTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void UpdateTargetLocation(const FVector& OwnerLocation);

	FVector TargetLocation;
	TArray<FVector> PathPoints;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	float AcceptanceRadius = 5.0f;
	
	/** Grid size for path building. Should be > 10 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI", meta=(ClampMin=11.0f))
	float PathGridSize = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	float ToleranceForPathPointsComparison = 100.0f;

public:
	UBTTask_FlyTo();
};

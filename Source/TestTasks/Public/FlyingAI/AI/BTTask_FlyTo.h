#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyTo.generated.h"

/**
 * Fly To task node.
 * Flies the AI pawn toward the specified Actor or Location blackboard entry using the A* pathfinding.
 * AI Pawn should have PawnMovementComponent or any other component, derived from PawnMovementComponent.
 */
UCLASS()
class TESTTASKS_API UBTTask_FlyTo : public UBTTaskNode
{
	GENERATED_BODY()

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void UpdatePathPoints(const UBehaviorTreeComponent& OwnerComp);
	void UpdateTargetLocation(const FVector& OwnerLocation);

	UFUNCTION()
	void OnPathPointsFound(const TArray<FVector>& Points);

	bool bSearchingPath = false;

	FVector TargetLocation;
	TArray<FVector> PathPoints;
	
	FVector EndLocation;
	bool bDestinationReached = false;

	EBlackboardNotificationResult OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

protected:
	UPROPERTY(EditAnywhere, Category="Flying AI")
	float AcceptanceRadius = 5.0f;
	
	/** Grid size for path building. Should be > 10 */
	UPROPERTY(EditAnywhere, Category="Flying AI", meta=(ClampMin=11.0f))
	float PathGridSize = 100.0f;

	UPROPERTY(EditAnywhere, Category="Flying AI")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleObjectTypes;

	UPROPERTY(EditAnywhere, Category="Flying AI")
	float ToleranceForPathPointsComparison = 100.0f;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetActorBlackboardKey;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetLocationBlackboardKey;

	/** If move goal in BB changes the move will be redirected to new location */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	bool bObserveBlackboardValue = true;

	/** If task is expected to react to changes to location represented by BB key 
	 *	this property can be used to tweak sensitivity of the mechanism. Value is 
	 *	recommended to be less than AcceptableRadius */
	UPROPERTY(EditAnywhere, Category="Blackboard", meta = (ClampMin = "1", UIMin = "1", EditCondition="bObserveBlackboardValue", DisplayAfter="bObserveBlackboardValue"))
	float ObservedBlackboardValueTolerance = 5.0f;

public:
	UBTTask_FlyTo();
};

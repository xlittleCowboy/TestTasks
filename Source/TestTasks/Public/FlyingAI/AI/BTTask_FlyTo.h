// Copyright littleCowboy. All Rights Reserved.

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

	UPROPERTY()
	UBehaviorTreeComponent* OwnerBehaviorTreeComponentComponent = nullptr;
	
	bool bSearchingForPath = false;

	FVector NextPathPointLocation;
	TArray<FVector> PathPoints;
	
	FVector EndLocation;
	bool bEndLocationReached = false;

	FTimerHandle ObserveActorLocationTimerHandle;
	void UpdatePathFromTargetActor();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void UpdatePathPoints(const UBehaviorTreeComponent& OwnerComp);
	void UpdateNextPathPointLocation(const FVector& OwnerLocation);

	UFUNCTION()
	void OnPathPointsFound(const TArray<FVector>& Points);

	EBlackboardNotificationResult OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

protected:
	/** What radius around end point should be acceptable for us to stop moving? */
	UPROPERTY(EditAnywhere, Category="Flying AI|Pathfinding")
	float AcceptanceRadius = 5.0f;
	
	/** Grid size for path building. Should be > 10. Larger values - better performance. */
	UPROPERTY(EditAnywhere, Category="Flying AI|Pathfinding", meta=(ClampMin=11.0f))
	float PathGridSize = 100.0f;

	/** Path consists of points. When we are moving to the point, what radius around it should be acceptable for us to move to the next point? */
	UPROPERTY(EditAnywhere, Category="Flying AI|Pathfinding")
	float ToleranceForPathPointsComparison = 100.0f;
	
	/** In which objects can't we fly? */
	UPROPERTY(EditAnywhere, Category="Flying AI|Pathfinding")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleObjectTypes;

	/** Should we update path to TargetActor every ObserveTargetActorLocationFrequency seconds? */
	UPROPERTY(EditAnywhere, Category="Flying AI|Observing")
	bool bObserveTargetActorLocation = false;

	/** If bObserveTargetActorLocation is true, then we update path to TargetActor every ObserveTargetActorLocationFrequency seconds. */
	UPROPERTY(EditAnywhere, Category="Flying AI|Observing", meta=(EditCondition="bObserveTargetActorLocation", ClampMin=0.1f, UIMin=0.1f))
	float ObserveTargetActorLocationFrequency = 2.0f;

	/** Actor we should move to. If this is null, then we will move to TargetLocationBlackboardKey. */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetActorBlackboardKey;

	/** Location we should move to. We move to it only if TargetLocationBlackboardKey is null. */
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

#include "FlyingAI/AI/BTTask_FlyTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "FlyingAI/AI/AStarPathfinding.h"
#include "GameFramework/PawnMovementComponent.h"

UBTTask_FlyTo::UBTTask_FlyTo()
{
	NodeName = "Fly To";
}

EBTNodeResult::Type UBTTask_FlyTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* AIBlackboard = OwnerComp.GetBlackboardComponent();
	const AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController || !AIBlackboard || !AIController->GetPawn())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	auto* PawnMovementComponent = AIController->GetPawn()->GetComponentByClass<UPawnMovementComponent>();
	if (!PawnMovementComponent)
	{
		UE_LOG(LogTemp, Fatal, TEXT("AI Fly To: Owner has no pawn movement component."))
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	if (PathPoints.Num() <= 0)
	{
		UObject* KeyValue = AIBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		const AActor* TargetActor = Cast<AActor>(KeyValue);
		if (!TargetActor)
		{
			UE_LOG(LogTemp, Fatal, TEXT("AI Fly To: Target actor is not set in blackboard."))
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
		
		PathPoints = UAStarPathfinding::GetPathPoints(AIController, AIController->GetPawn()->GetActorLocation(),
			TargetActor->GetActorLocation(), ObstacleObjectTypes, PathGridSize);

		if (PathPoints.Num() <= 0)
		{
			UE_LOG(LogTemp, Fatal, TEXT("AI Fly To: No path points."))
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}

		TargetLocation = PathPoints[0];
		PathPoints.RemoveAt(0);
	}

	UpdateTargetLocation(AIController->GetPawn()->GetActorLocation());
	PawnMovementComponent->AddInputVector((TargetLocation - AIController->GetPawn()->GetActorLocation()).GetSafeNormal());
	
	if (PathPoints.Num() <= 0 && AIController->GetPawn()->GetActorLocation().Equals(TargetLocation, AcceptanceRadius))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::InProgress;
}

void UBTTask_FlyTo::UpdateTargetLocation(const FVector& OwnerLocation)
{
	if (PathPoints.Num() <= 0)
	{
		return;
	}

	if (OwnerLocation.Equals(TargetLocation, ToleranceForPathPointsComparison))
	{
		TargetLocation = PathPoints[0];
		PathPoints.RemoveAt(0);
	}
}

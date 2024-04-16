#include "FlyingAI/AI/BTTask_FlyTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "FlyingAI/AI/AStarPathfinding.h"
#include "GameFramework/PawnMovementComponent.h"

UBTTask_FlyTo::UBTTask_FlyTo()
{
	NodeName = "Fly To";
}

void UBTTask_FlyTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetActorBlackboardKey.ResolveSelectedKey(*BBAsset);
		TargetLocationBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_FlyTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* AIBlackboard = OwnerComp.GetBlackboardComponent();
	const AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController || !AIBlackboard || !AIController->GetPawn())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	auto* PawnMovementComponent = AIController->GetPawn()->GetComponentByClass<UPawnMovementComponent>();
	if (!PawnMovementComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Fly To: Owner has no pawn movement component."))
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	if (PathPoints.Num() <= 0 && !bDestinationReached && !bSearchingPath)
	{
		UpdatePathPoints(OwnerComp);
	}

	UpdateTargetLocation(AIController->GetPawn()->GetActorLocation());
	PawnMovementComponent->AddInputVector((TargetLocation - AIController->GetPawn()->GetActorLocation()).GetSafeNormal());

	if (bObserveBlackboardValue)
	{
		AIBlackboard->UnregisterObserversFrom(this);
		
		AIBlackboard->RegisterObserver(TargetActorBlackboardKey.GetSelectedKeyID(),
			this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_FlyTo::OnBlackboardValueChange));
		AIBlackboard->RegisterObserver(TargetLocationBlackboardKey.GetSelectedKeyID(),
			this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_FlyTo::OnBlackboardValueChange));
	}	
	
	if (bDestinationReached)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::InProgress;
}

void UBTTask_FlyTo::UpdatePathPoints(const UBehaviorTreeComponent& OwnerComp)
{
	const UBlackboardComponent* AIBlackboard = OwnerComp.GetBlackboardComponent();
	const AAIController* AIController = OwnerComp.GetAIOwner();
	
	UObject* KeyValue = AIBlackboard->GetValue<UBlackboardKeyType_Object>(TargetActorBlackboardKey.SelectedKeyName);
	const AActor* TargetActor = Cast<AActor>(KeyValue);
	if (!TargetActor && !AIBlackboard->IsVectorValueSet(TargetLocationBlackboardKey.SelectedKeyName))
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Fly To: Target actor or target location is not set in the blackboard."))
		return;
	}

	bSearchingPath = true;

	FPathPointsDelegate PathPointsDelegate;
	PathPointsDelegate.BindUFunction(this, "OnPathPointsFound");
	
	if (TargetActor)
	{
		UAStarPathfinding::GetPathPoints(AIController, PathPointsDelegate, AIController->GetPawn()->GetActorLocation(),
			TargetActor->GetActorLocation(), ObstacleObjectTypes, PathGridSize);
	}
	else
	{
		const FVector TargetEndLocation = AIBlackboard->GetValue<UBlackboardKeyType_Vector>(TargetLocationBlackboardKey.SelectedKeyName);

		UAStarPathfinding::GetPathPoints(AIController, PathPointsDelegate, AIController->GetPawn()->GetActorLocation(),
	TargetEndLocation, ObstacleObjectTypes, PathGridSize);
	}
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

		if (PathPoints.Num() <= 0)
		{
			bDestinationReached = true;
		}
	}
}

void UBTTask_FlyTo::OnPathPointsFound(const TArray<FVector>& Points)
{
	bSearchingPath = false;
	
	PathPoints = Points;

	if (PathPoints.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Fly To: No path points."))
		return;
	}

	TargetLocation = PathPoints[0];
	EndLocation = PathPoints[PathPoints.Num() - 1];
		
	PathPoints.RemoveAt(0);
}

EBlackboardNotificationResult UBTTask_FlyTo::OnBlackboardValueChange(const UBlackboardComponent& Blackboard,
                                                                     FBlackboard::FKey ChangedKeyID)
{
	const UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());
	if (!BehaviorComp)
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	bool bUpdateMove = false;
	FVector TargetEndLocation;
	if (ChangedKeyID == TargetActorBlackboardKey.GetSelectedKeyID())
	{
		UObject* KeyValue = Blackboard.GetValue<UBlackboardKeyType_Object>(TargetActorBlackboardKey.SelectedKeyName);
		if (const AActor* TargetActor = Cast<AActor>(KeyValue))
		{
			TargetEndLocation = TargetActor->GetActorLocation();
			bUpdateMove = FVector::DistSquared(EndLocation, TargetEndLocation) > FMath::Square(ObservedBlackboardValueTolerance);
		}
	}
	if (ChangedKeyID == TargetLocationBlackboardKey.GetSelectedKeyID())
	{
		TargetEndLocation = Blackboard.GetValue<UBlackboardKeyType_Vector>(TargetLocationBlackboardKey.SelectedKeyName);
		bUpdateMove = FVector::DistSquared(EndLocation, TargetEndLocation) > FMath::Square(ObservedBlackboardValueTolerance);
	}

	if (bUpdateMove && BehaviorComp->GetAIOwner())
	{
		FPathPointsDelegate PathPointsDelegate;
		PathPointsDelegate.BindUFunction(this, "OnPathPointsFound");
		
		UAStarPathfinding::GetPathPoints(BehaviorComp->GetAIOwner(), PathPointsDelegate, BehaviorComp->GetAIOwner()->GetPawn()->GetActorLocation(),
	TargetEndLocation, ObstacleObjectTypes, PathGridSize);

		if (PathPoints.Num() > 0)
		{
			TargetLocation = PathPoints[0];
			EndLocation = TargetEndLocation;

			PathPoints.RemoveAt(0);

			bDestinationReached = false;
		}
	}
	
	return EBlackboardNotificationResult::ContinueObserving;
}

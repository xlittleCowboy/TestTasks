#include "FlyingAI/AI/FlyingAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool AFlyingAIController::FlyToActor(AActor* TargetActor)
{
	check(FlyingAIBlackboard);
	check(FlyingAIBehaviorTree);
	
	if (!TargetActor)
	{
		return false;
	}

	UBlackboardComponent* BlackboardComponent;
	UseBlackboard(FlyingAIBlackboard, BlackboardComponent);

	if (!BlackboardComponent)
	{
		return false;
	}

	UpdateFlyToActor(TargetActor);

	return RunBehaviorTree(FlyingAIBehaviorTree);
}

bool AFlyingAIController::FlyToLocation(const FVector& TargetLocation)
{
	check(FlyingAIBlackboard);
	check(FlyingAIBehaviorTree);
	
	UBlackboardComponent* BlackboardComponent;
	UseBlackboard(FlyingAIBlackboard, BlackboardComponent);

	if (!BlackboardComponent)
	{
		return false;
	}
	
	UpdateFlyToLocation(TargetLocation);
	
	return RunBehaviorTree(FlyingAIBehaviorTree);
}

bool AFlyingAIController::UpdateFlyToActor(AActor* TargetActor)
{
	if (GetBlackboardComponent())
	{
		OnTargetActorChanged.Broadcast(TargetActor);

		GetBlackboardComponent()->SetValueAsObject(TargetActorBlackboardKeyName, TargetActor);

		return true;
	}

	return false;
}

bool AFlyingAIController::UpdateFlyToLocation(const FVector& TargetLocation)
{
	if (GetBlackboardComponent())
	{
		OnTargetLocationChanged.Broadcast(TargetLocation);

		GetBlackboardComponent()->SetValueAsVector(TargetLocationBlackboardKeyName, TargetLocation);

		return true;
	}

	return false;
}

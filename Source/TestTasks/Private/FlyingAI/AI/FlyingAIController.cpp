#include "FlyingAI/AI/FlyingAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool AFlyingAIController::FlyToActor(AActor* TargetActor)
{
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
	
	BlackboardComponent->SetValueAsObject(TargetActorBlackboardKeyName, TargetActor);

	return RunBehaviorTree(FlyingAIBehaviorTree);
}

bool AFlyingAIController::FlyToLocation(const FVector& TargetLocation)
{
	UBlackboardComponent* BlackboardComponent;
	UseBlackboard(FlyingAIBlackboard, BlackboardComponent);

	if (!BlackboardComponent)
	{
		return false;
	}
	
	BlackboardComponent->SetValueAsVector(TargetLocationBlackboardKeyName, TargetLocation);
	
	return RunBehaviorTree(FlyingAIBehaviorTree);
}

bool AFlyingAIController::UpdateFlyToActor(AActor* TargetActor)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetActorBlackboardKeyName, TargetActor);

		return true;
	}

	return false;
}

bool AFlyingAIController::UpdateFlyToLocation(const FVector& TargetLocation)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsVector(TargetLocationBlackboardKeyName, TargetLocation);

		return true;
	}

	return false;
}

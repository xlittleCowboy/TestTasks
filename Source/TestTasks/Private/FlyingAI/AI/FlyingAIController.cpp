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

	OnTargetActorChanged.Broadcast(TargetActor);
	
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
	
	OnTargetLocationChanged.Broadcast(TargetLocation);
	
	BlackboardComponent->SetValueAsVector(TargetLocationBlackboardKeyName, TargetLocation);
	
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

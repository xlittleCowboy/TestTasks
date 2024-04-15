#include "FlyingAI/AI/FlyingAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool AFlyingAIController::FlyTo(AActor* TargetActor)
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
	
	BlackboardComponent->SetValueAsObject("TargetActor", TargetActor);
	
	return RunBehaviorTree(FlyingAIBehaviourTree);
}

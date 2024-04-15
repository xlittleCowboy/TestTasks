#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FlyingAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKS_API AFlyingAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	UBlackboardData* FlyingAIBlackboard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	UBehaviorTree* FlyingAIBehaviourTree;

public:
	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool FlyTo(AActor* TargetActor);
};

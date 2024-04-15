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
	FName TargetActorBlackboardKeyName = "TargetActor";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	FName TargetLocationBlackboardKeyName = "TargetLocation";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	UBehaviorTree* FlyingAIBehaviorTree;

public:
	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool FlyToActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool FlyToLocation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool UpdateFlyToActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool UpdateFlyToLocation(const FVector& TargetLocation);
};

// Copyright littleCowboy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FlyingAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetActorChangedDelegate, AActor*, NewActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLocationChangedDelegate, FVector, NewLocation);

/**
 * 
 */
UCLASS()
class TESTTASKS_API AFlyingAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	TObjectPtr<UBlackboardData> FlyingAIBlackboard = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	FName TargetActorBlackboardKeyName = "TargetActor";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	FName TargetLocationBlackboardKeyName = "TargetLocation";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying AI")
	TObjectPtr<UBehaviorTree> FlyingAIBehaviorTree = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool FlyToActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool FlyToLocation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool UpdateFlyToActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Flying AI")
	bool UpdateFlyToLocation(const FVector& TargetLocation);

	UPROPERTY(BlueprintAssignable)
	FOnTargetActorChangedDelegate OnTargetActorChanged;

	UPROPERTY(BlueprintAssignable)
	FOnTargetLocationChangedDelegate OnTargetLocationChanged;
};

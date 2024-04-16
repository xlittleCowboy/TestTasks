#pragma once

#include "CoreMinimal.h"
#include "FlyingAI/AI/FlyingAIController.h"
#include "GameFramework/Pawn.h"
#include "FlyingPawn.generated.h"

class UFloatingPawnMovement;

UCLASS()
class TESTTASKS_API AFlyingPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UFloatingPawnMovement* FloatingPawnMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn")
	float RotationAlpha = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying Pawn")
	bool bFlyToTargetOnBeginPlay = true;

	/** If TargetActor is not set, then pawn will fly to the TargetLocation. */
	UPROPERTY(ReplicatedUsing="OnRep_TargetActor", EditInstanceOnly, BlueprintReadOnly, Category="Flying Pawn", meta=(EditCondition="bFlyToTargetOnBeginPlay", EditConditionHides="true"))
	AActor* TargetActor = nullptr;

	/** If TargetActor is not set, then pawn will fly to the TargetLocation. */
	UPROPERTY(ReplicatedUsing="OnRep_TargetLocation", EditInstanceOnly, BlueprintReadOnly, Category="Flying Pawn", meta=(EditCondition="bFlyToTargetOnBeginPlay && TargetActor == nullptr", EditConditionHides="true"))
	FVector TargetLocation;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_TargetActor();

	UFUNCTION()
	void OnRep_TargetLocation();

public:
	AFlyingPawn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable)
	FOnTargetActorChangedDelegate OnTargetActorChanged;

	UPROPERTY(BlueprintAssignable)
	FOnTargetLocationChangedDelegate OnTargetLocationChanged;

	UFUNCTION()
	void OnTargetActorChangedCallback(AActor* NewActor);

	UFUNCTION()
	void OnTargetLocationChangedCallback(FVector NewLocation);
};

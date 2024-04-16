#include "FlyingAI/Actors/FlyingPawn.h"
#include "FlyingAI/AI/AStarPathfinding.h"
#include "FlyingAI/AI/FlyingAIController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

AFlyingPawn::AFlyingPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Floating Pawn Movement");
}

void AFlyingPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetVelocity().SquaredLength() > 10000.0f)
	{
		FRotator EndRotation = GetActorRotation();
		EndRotation.Yaw = GetVelocity().Rotation().Yaw;
	
		SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), EndRotation, RotationAlpha, true));
	}
}

void AFlyingPawn::OnPathPointsFound(const TArray<FVector>& Points)
{
	UAStarPathfinding::DrawPath(GetWorld(), Points);
}

void AFlyingPawn::BeginPlay()
{
	Super::BeginPlay();

	if (auto* FlyingAIController = Cast<AFlyingAIController>(GetController()); FlyingAIController && bFlyToTargetOnBeginPlay)
	{
		if (TargetActor)
		{
			FlyingAIController->FlyToActor(TargetActor);
		}
		else
		{
			FlyingAIController->FlyToLocation(TargetLocation);
		}
	}
}
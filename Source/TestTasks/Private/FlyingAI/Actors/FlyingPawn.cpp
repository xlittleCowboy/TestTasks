#include "FlyingAI/Actors/FlyingPawn.h"
#include "FlyingAI/AI/AStarPathfinding.h"
#include "FlyingAI/AI/FlyingAIController.h"
#include "GameFramework/FloatingPawnMovement.h"

AFlyingPawn::AFlyingPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Floating Pawn Movement");
}

void AFlyingPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	FlushPersistentDebugLines(GetWorld());
	
	if (bDrawDebugPath && TargetActor)
	{
		DrawDebugPath(UAStarPathfinding::GetPathPoints(GetWorld(), GetActorLocation(), TargetActor->GetActorLocation(), ObstacleObjectTypes, PathGridSize));
	}
}

void AFlyingPawn::DrawDebugPath(const TArray<FVector>& Path) const
{
	if (!GetWorld())
	{
		return;
	}
	
	for (int32 i = 0; i < Path.Num() - 1; i++)
	{
		DrawDebugLine(GetWorld(),Path[i], Path[i + 1], FColor::Green,
			false, std::numeric_limits<float>::max(), 0, 1.0f);
	}
}

void AFlyingPawn::BeginPlay()
{
	Super::BeginPlay();

	if (auto* FlyingAIController = Cast<AFlyingAIController>(GetController()); FlyingAIController && bFlyToTargetActorOnBeginPlay)
	{
		FlyingAIController->FlyTo(TargetActor);
	}
}
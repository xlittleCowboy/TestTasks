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

void AFlyingPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	FlushPersistentDebugLines(GetWorld());
	
	if (bDrawDebugPath)
	{
		FVector EndLocation = TargetLocation;
		if (TargetActor)
		{
			EndLocation = TargetActor->GetActorLocation();
		}
		
		DrawDebugPath(UAStarPathfinding::GetPathPoints(GetWorld(), GetActorLocation(), EndLocation, ObstacleObjectTypes, PathGridSize));
	}
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
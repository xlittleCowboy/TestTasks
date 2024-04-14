#include "Drone/Drone.h"
#include "Algo/Reverse.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADrone::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FlushPersistentDebugLines(GetWorld());
	
	if (bDrawDebugPath && TargetActor)
	{
		DrawPath(GetPathPoints(GetActorLocation(), TargetActor->GetActorLocation()));
	}
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FVector> ADrone::GetPathPoints(const FVector& FromLocation, const FVector& ToLocation) const
{
	TArray<FNavigationNode> OpenNodes;
	TArray<FNavigationNode> ClosedNodes;

	const FNavigationNode StartNode = FNavigationNode(UKismetMathLibrary::Vector_SnappedToGrid(FromLocation, NeighbourNodesDistance));
	const FNavigationNode EndNode = FNavigationNode(GetValidEndLocation(ToLocation));
	
	OpenNodes.Add(StartNode);

	while (OpenNodes.Num() > 0)
	{
		int32 CurrentNodeIndex = 0;
		FNavigationNode CurrentNode = GetNodeWithLowestFCost(OpenNodes, CurrentNodeIndex);
		
		OpenNodes.RemoveAt(CurrentNodeIndex);
		const int32 CurrentClosedNodeIndex = ClosedNodes.Add(CurrentNode);

		if (CurrentNode == EndNode)
		{
			TArray<FVector> PathPoints;
			PathPoints.Add(ToLocation);

			while (true)
			{
				PathPoints.Add(CurrentNode.Location);

				if (CurrentNode.ParentNodeIndex < 0)
				{
					break;
				}
				
				CurrentNode = ClosedNodes[CurrentNode.ParentNodeIndex];
			}

			Algo::Reverse(PathPoints);
			return PathPoints;
		}

		for (const auto& Neighbour : GetNeighbourNodes(CurrentNode))
		{
			if (ClosedNodes.Find(Neighbour) >= 0)
			{
				continue;
			}

			int32 NeighbourOpenNodeIndex = OpenNodes.Find(Neighbour);
			const float NewGCostToNeighbour = CurrentNode.G + FVector::Distance(Neighbour.Location, CurrentNode.Location);
			
			if (NewGCostToNeighbour < Neighbour.G || NeighbourOpenNodeIndex < 0)
			{
				if (NeighbourOpenNodeIndex < 0)
				{
					NeighbourOpenNodeIndex = OpenNodes.Add(Neighbour);
				}
				
				OpenNodes[NeighbourOpenNodeIndex].G = NewGCostToNeighbour;
				OpenNodes[NeighbourOpenNodeIndex].H = FVector::Distance(EndNode.Location, Neighbour.Location);
				OpenNodes[NeighbourOpenNodeIndex].F = Neighbour.G + Neighbour.H;
				OpenNodes[NeighbourOpenNodeIndex].ParentNodeIndex = CurrentClosedNodeIndex;
			}
		}
	}
	
	return {};
}

FNavigationNode ADrone::GetNodeWithLowestFCost(const TArray<FNavigationNode>& Nodes, int32& Index) const
{
	if (Nodes.Num() <= 0)
	{
		return {};
	}
	
	FNavigationNode LowestFCostNode = Nodes[0];
	for (int32 i = 1; i < Nodes.Num(); i++)
	{
		if (Nodes[i].F < LowestFCostNode.F || (FMath::IsNearlyEqual(Nodes[i].F, LowestFCostNode.F, 0.1f) && Nodes[i].H < LowestFCostNode.H))
		{
			LowestFCostNode = Nodes[i];
			Index = i;
		}
	}

	return LowestFCostNode;
}

TArray<FNavigationNode> ADrone::GetNeighbourNodes(const FNavigationNode& Node) const
{
	TArray<FNavigationNode> NeighbourNodes;
	
	for (int32 i = -1; i < 2; i++)
	{
		for (int32 j = -1; j < 2; j++)
		{
			for (int32 k = -1; k < 2; k++)
			{
				if (i == 0 && j == 0 && k == 0)
				{
					continue;
				}
				
				FVector NeighbourLocation = Node.Location;
				NeighbourLocation.X += i * NeighbourNodesDistance;
				NeighbourLocation.Y += j * NeighbourNodesDistance;
				NeighbourLocation.Z += k * NeighbourNodesDistance;
				
				if (IsLocationValidForNode(NeighbourLocation))
				{
					NeighbourNodes.Add(FNavigationNode(NeighbourLocation));
				}
			}
		}
	}

	return NeighbourNodes;
}

FVector ADrone::GetValidEndLocation(const FVector& EndLocation) const
{
	TArray<FVector> ValidLocations;

	int32 GridSize = 1;

	while (true)
	{
		for (int32 i = -GridSize; i <= GridSize; i++)
		{
			for (int32 j = -GridSize; j <= GridSize; j++)
			{
				for (int32 k = -GridSize; k <= GridSize; k++)
				{
					FVector ValidEndLocation = UKismetMathLibrary::Vector_SnappedToGrid(EndLocation, NeighbourNodesDistance);
					ValidEndLocation.X += i * NeighbourNodesDistance;
					ValidEndLocation.Y += j * NeighbourNodesDistance;
					ValidEndLocation.Z += k * NeighbourNodesDistance;
				
					if (IsLocationValidForNode(ValidEndLocation))
					{
						ValidLocations.Add(ValidEndLocation);
					}
				}
			}
		}

		if (ValidLocations.Num() > 0)
		{
			ValidLocations.Sort([&](const FVector& Location, const FVector& OtherLocation)
				{ return (Location - EndLocation).Length() <= (OtherLocation - EndLocation).Length(); });
			
			return ValidLocations[0];
		}

		GridSize++;
	}
}

bool ADrone::IsLocationValidForNode(const FVector& Location) const
{
	const TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	return !UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, NeighbourNodesDistance / 2.0f,
		ObstaclesObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors);
}

void ADrone::DrawPath(const TArray<FVector>& Path)
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

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

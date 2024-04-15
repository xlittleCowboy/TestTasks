#include "FlyingAI/AI/AStarPathfinding.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAStarPathfinding::GetPathPoints(const UObject* WorldContextObject, const FVector& FromLocation, const FVector& ToLocation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, float GridSize)
{
	TArray<FNavigationNode> OpenNodes;
	TArray<FNavigationNode> ClosedNodes;

	const FNavigationNode StartNode = FNavigationNode(UKismetMathLibrary::Vector_SnappedToGrid(FromLocation, GridSize));
	const FNavigationNode EndNode = FNavigationNode(GetValidEndLocation(WorldContextObject, ToLocation, ObstacleObjectTypes, GridSize));
	
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

			// Add correct final location. Our target actor could be unreachable, so we do line trace and find reachable final location.
			FVector EndLocation = ToLocation;
			TArray<AActor*> ActorsToIgnore;
			FHitResult OutHit;
			
			if (UKismetSystemLibrary::LineTraceSingle(WorldContextObject, EndNode.Location, ToLocation, UEngineTypes::ConvertToTraceType(ECC_Visibility),
				true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true))
			{
				EndLocation = OutHit.Location;
			}
			
			PathPoints.Add(EndLocation);

			while (true)
			{
				PathPoints.Add(CurrentNode.Location);

				if (CurrentNode.ParentNodeIndex < 0)
				{
					break;
				}
				
				CurrentNode = ClosedNodes[CurrentNode.ParentNodeIndex];
			}

			// We are building a path on the grid, but our start location not always could be snapped to a grid.
			// So we delete start grid location and replace it with our actual start location.
			if (PathPoints.Num() > 0)
			{
				PathPoints.RemoveAt(PathPoints.Num() - 1);
				PathPoints.Add(FromLocation);
			}
			
			Algo::Reverse(PathPoints);
			
			return PathPoints;
		}

		for (const auto& Neighbour : GetNeighbourNodes(WorldContextObject, CurrentNode, ObstacleObjectTypes, GridSize))
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

FNavigationNode UAStarPathfinding::GetNodeWithLowestFCost(const TArray<FNavigationNode>& Nodes, int32& Index)
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

TArray<FNavigationNode> UAStarPathfinding::GetNeighbourNodes(const UObject* WorldContextObject, const FNavigationNode& Node, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, const float GridSize)
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
				NeighbourLocation.X += i * GridSize;
				NeighbourLocation.Y += j * GridSize;
				NeighbourLocation.Z += k * GridSize;
				
				if (IsLocationValidForNode(WorldContextObject, NeighbourLocation, ObstacleObjectTypes, GridSize))
				{
					NeighbourNodes.Add(FNavigationNode(NeighbourLocation));
				}
			}
		}
	}

	return NeighbourNodes;
}

FVector UAStarPathfinding::GetValidEndLocation(const UObject* WorldContextObject, const FVector& EndLocation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, const float GridSize)
{
	TArray<FVector> ValidLocations;

	int32 OutGridSize = 1;

	while (true)
	{
		for (int32 i = -OutGridSize; i <= OutGridSize; i++)
		{
			for (int32 j = -OutGridSize; j <= OutGridSize; j++)
			{
				for (int32 k = -OutGridSize; k <= OutGridSize; k++)
				{
					FVector ValidEndLocation = UKismetMathLibrary::Vector_SnappedToGrid(EndLocation, GridSize);
					ValidEndLocation.X += i * GridSize;
					ValidEndLocation.Y += j * GridSize;
					ValidEndLocation.Z += k * GridSize;
				
					if (IsLocationValidForNode(WorldContextObject, ValidEndLocation, ObstacleObjectTypes, GridSize))
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

		OutGridSize++;
	}
}

bool UAStarPathfinding::IsLocationValidForNode(const UObject* WorldContextObject, const FVector& Location, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, const float GridSize)
{
	const TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	return !UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, Location, GridSize / 2.0f,
		ObstacleObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors);
}
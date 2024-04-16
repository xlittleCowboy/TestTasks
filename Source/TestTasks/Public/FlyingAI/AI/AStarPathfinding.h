#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AStarPathfinding.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FPathPointsDelegate, TArray<FVector>, PathPoints);

USTRUCT(BlueprintType)
struct FNavigationNode
{
	GENERATED_BODY()

	/** Distance from start node to current node */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float G = 0.0f;

	/** Distance from current node to destination node */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float H = 0.0f;

	/** G + H */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float F = 0.0f;

	/** Node world location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ParentNodeIndex = -1;

	FNavigationNode() {}
	FNavigationNode(const FVector& InLocation) : Location(InLocation) {}
	FNavigationNode(float InF) : F(InF) {}

	FORCEINLINE bool operator== (const FNavigationNode& OtherNode) const { return Location.Equals(OtherNode.Location, 10.0f); }

	FORCEINLINE bool operator< (const FNavigationNode& OtherNode) const
	{
		if (FMath::IsNearlyEqual(F, OtherNode.F, 1.0f))
		{
			return H < OtherNode.H;
		}
		
		return F < OtherNode.F;
	}
};

/**
 * 
 */
UCLASS()
class TESTTASKS_API UAStarPathfinding : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Pathfinding", meta = (WorldContext = "WorldContextObject"))
	static void GetPathPoints(const UObject* WorldContextObject, FPathPointsDelegate Out, const FVector& FromLocation, const FVector& ToLocation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, float GridSize);

	UFUNCTION(BlueprintPure, Category="Pathfinding", meta = (WorldContext = "WorldContextObject"))
	static TArray<FNavigationNode> GetNeighbourNodes(const UObject* WorldContextObject, const FNavigationNode& Node, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, float GridSize);

	UFUNCTION(BlueprintPure, Category="Pathfinding", meta = (WorldContext = "WorldContextObject"))
	static bool IsLocationValidForNode(const UObject* WorldContextObject, const FVector& Location, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, float GridSize);

	UFUNCTION(BlueprintPure, Category="Pathfinding", meta = (WorldContext = "WorldContextObject"))
	static FVector GetValidEndLocation(const UObject* WorldContextObject, const FVector& EndLocation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObstacleObjectTypes, float GridSize);

	UFUNCTION(BlueprintPure, Category="Pathfinding", meta = (WorldContext = "WorldContextObject"))
	static void DrawPath(const UObject* WorldContextObject, const TArray<FVector>& PathPoints);
};

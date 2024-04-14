#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

USTRUCT()
struct FNavigationNode
{
	GENERATED_BODY()

	/** Distance from start node to current node */
	UPROPERTY()
	float G = 0.0f;

	/** Distance from current node to destination node */
	UPROPERTY()
	float H = 0.0f;

	/** G + H */
	UPROPERTY()
	float F = 0.0f;

	/** Node world location */
	UPROPERTY()
	FVector Location = FVector::ZeroVector;

	UPROPERTY()
	int32 ParentNodeIndex = -1;

	FNavigationNode() {}
	FNavigationNode(const FVector& InLocation) : Location(InLocation) {}

	FORCEINLINE bool operator== (const FNavigationNode& OtherNode) const { return Location.Equals(OtherNode.Location, 10.0f); }
};

UCLASS()
class TESTTASKS_API ADrone : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Drone", meta=(ClampMin=11.0f))
	float NeighbourNodesDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Drone")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstaclesObjectTypes;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Drone")
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Drone")
	bool bDrawDebugPath = false;

protected:
	virtual void BeginPlay() override;
	
	TArray<FVector> GetPathPoints(const FVector& FromLocation, const FVector& ToLocation) const;

	FNavigationNode GetNodeWithLowestFCost(const TArray<FNavigationNode>& Nodes, int32& Index) const;

	bool IsLocationValidForNode(const FVector& Location) const;
	
	TArray<FNavigationNode> GetNeighbourNodes(const FNavigationNode& Node) const;

	FVector GetValidEndLocation(const FVector& EndLocation) const;
	
	void DrawPath(const TArray<FVector>& Path);

public:
	ADrone();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void Tick(float DeltaTime) override;
};

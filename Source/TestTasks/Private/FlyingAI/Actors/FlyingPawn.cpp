#include "FlyingAI/Actors/FlyingPawn.h"
#include "FlyingAI/AI/FlyingAIController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AFlyingPawn::AFlyingPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Floating Pawn Movement");
}

void AFlyingPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlyingPawn, TargetActor);
	DOREPLIFETIME(AFlyingPawn, TargetLocation);
}

void AFlyingPawn::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (auto* FlyingAIController = Cast<AFlyingAIController>(GetController()); FlyingAIController && bFlyToTargetOnBeginPlay)
		{
			FlyingAIController->OnTargetActorChanged.AddDynamic(this, &AFlyingPawn::OnTargetActorChangedCallback);
			FlyingAIController->OnTargetLocationChanged.AddDynamic(this, &AFlyingPawn::OnTargetLocationChangedCallback);
			
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

void AFlyingPawn::OnTargetActorChangedCallback(AActor* NewActor)
{
	TargetActor = NewActor;
	OnRep_TargetActor();
}

void AFlyingPawn::OnTargetLocationChangedCallback(FVector NewLocation)
{
	TargetLocation = NewLocation;
	OnRep_TargetLocation();
}

void AFlyingPawn::OnRep_TargetActor()
{
	OnTargetActorChanged.Broadcast(TargetActor);
}

void AFlyingPawn::OnRep_TargetLocation()
{
	OnTargetLocationChanged.Broadcast(TargetLocation);	
}
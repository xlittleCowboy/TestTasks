// Copyright littleCowboy. All Rights Reserved.

#include "Electricity/ElectricalDeviceComponent.h"

#include "Electricity/ElectricalDeviceDefinition.h"

UElectricalDeviceComponent::UElectricalDeviceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UElectricalDeviceComponent::BeginPlay()
{
	Super::BeginPlay();
}

UElectricalDeviceDefinition* UElectricalDeviceComponent::GetElectricalDeviceDefinitionCasted(
	const TSubclassOf<UElectricalDeviceDefinition> ElectricalDeviceDefinitionClass) const
{
	if (ElectricalDeviceDefinition && ElectricalDeviceDefinition->IsA(ElectricalDeviceDefinitionClass))
	{
		return ElectricalDeviceDefinition;
	}

	return nullptr;
}

UElectricalDeviceDefinition* UElectricalDeviceComponent::GetElectricalDeviceDefinitionFromActor(const AActor* Actor,
                                                                                                const TSubclassOf<UElectricalDeviceDefinition> ElectricalDeviceDefinitionClass)
{
	if (!Actor)
	{
		return nullptr;
	}
	
	if (const auto* ElectricalDeviceComponent = Actor->GetComponentByClass<UElectricalDeviceComponent>())
	{
		if (ElectricalDeviceComponent->GetElectricalDeviceDefinition()->IsA(ElectricalDeviceDefinitionClass))
		{
			return ElectricalDeviceComponent->GetElectricalDeviceDefinition();
		}
	}

	return nullptr;
}

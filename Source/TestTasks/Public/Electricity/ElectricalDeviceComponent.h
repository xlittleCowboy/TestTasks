// Copyright littleCowboy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ElectricalDeviceComponent.generated.h"


class UElectricalDeviceDefinition;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTTASKS_API UElectricalDeviceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UElectricalDeviceComponent();

	UFUNCTION(BlueprintPure)
	UElectricalDeviceDefinition* GetElectricalDeviceDefinition() const { return ElectricalDeviceDefinition; }

	UFUNCTION(BlueprintPure, Category="Inventory", meta=(DeterminesOutputType = "ElectricalDeviceDefinitionClass"))
	UElectricalDeviceDefinition* GetElectricalDeviceDefinitionCasted(TSubclassOf<UElectricalDeviceDefinition> ElectricalDeviceDefinitionClass) const;

	template<class T>
	T* GetElectricalDeviceDefinitionCasted()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UElectricalDeviceDefinition>::Value, "'T' template parameter to GetElectricalDeviceDefinitionFromActor must be derived from UElectricalDeviceDefinition");

		return Cast<T>(ElectricalDeviceDefinition);
	}

	UFUNCTION(BlueprintPure, Category="Inventory", meta=(DeterminesOutputType = "ElectricalDeviceDefinitionClass"))
	static UElectricalDeviceDefinition* GetElectricalDeviceDefinitionFromActor(const AActor* Actor,
	                                                                           TSubclassOf<UElectricalDeviceDefinition> ElectricalDeviceDefinitionClass);

	template<class T>
	static T* GetElectricalDeviceDefinitionFromActor(AActor* Actor)
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UElectricalDeviceDefinition>::Value, "'T' template parameter to GetElectricalDeviceDefinitionFromActor must be derived from UElectricalDeviceDefinition");

		return Cast<T>(GetElectricalDeviceDefinitionFromActor(Actor, T::StaticClass()));
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintGetter="GetElectricalDeviceDefinition", Instanced, Category="Electricity")
	TObjectPtr<UElectricalDeviceDefinition> ElectricalDeviceDefinition = nullptr;
};

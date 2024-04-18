// Copyright littleCowboy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElectricalDeviceDefinition.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable, BlueprintType)
class TESTTASKS_API UElectricalDeviceDefinition : public UObject
{
	GENERATED_BODY()

public:
	UElectricalDeviceDefinition();
};
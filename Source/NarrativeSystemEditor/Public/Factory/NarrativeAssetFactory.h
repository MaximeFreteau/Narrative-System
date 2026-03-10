// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "NarrativeAssetFactory.generated.h"

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UNarrativeAssetFactory();
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override { return true; }
	virtual FText GetDisplayName() const override;
	virtual uint32 GetMenuCategories() const override;
};
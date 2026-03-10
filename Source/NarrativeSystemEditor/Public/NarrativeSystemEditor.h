// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FNarrativeAssetTypeActions;

class FNarrativeSystemEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FNarrativeAssetTypeActions> NarrativeAssetTypeActions;
};
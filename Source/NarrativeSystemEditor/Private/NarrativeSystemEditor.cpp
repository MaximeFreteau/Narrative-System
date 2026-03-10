// Copyright Maxime Freteau. All Rights Reserved.

#include "NarrativeSystemEditor.h"
#include "Customization/NarrativeAssetTypeActions.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FNarrativeSystemEditorModule"

void FNarrativeSystemEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	NarrativeAssetTypeActions = MakeShared<FNarrativeAssetTypeActions>();
	AssetTools.RegisterAssetTypeActions(NarrativeAssetTypeActions.ToSharedRef());
}

void FNarrativeSystemEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		if (NarrativeAssetTypeActions.IsValid())
		{
			AssetTools.UnregisterAssetTypeActions(NarrativeAssetTypeActions.ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNarrativeSystemEditorModule, NarrativeSystemEditor)
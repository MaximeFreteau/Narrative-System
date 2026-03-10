// Copyright Maxime Freteau. All Rights Reserved.

#include "Customization/NarrativeAssetTypeActions.h"
#include "Core/NarrativeAsset.h"
#include "AssetEditor/NarrativeAssetEditorToolkit.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FNarrativeAssetTypeActions::GetName() const
{
	return LOCTEXT("NarrativeAssetName", "Narrative Asset");
}

FColor FNarrativeAssetTypeActions::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FNarrativeAssetTypeActions::GetSupportedClass() const
{
	return UNarrativeAsset::StaticClass();
}

void FNarrativeAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto Object : InObjects)
	{
		if (UNarrativeAsset* Asset = Cast<UNarrativeAsset>(Object))
		{
			TSharedRef<FNarrativeAssetEditorToolkit> EditorToolkit = MakeShared<FNarrativeAssetEditorToolkit>();
			EditorToolkit->InitEditor(Mode, EditWithinLevelEditor, Asset);
		}
	}
}

uint32 FNarrativeAssetTypeActions::GetCategories()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	return AssetTools.RegisterAdvancedAssetCategory("Narrative", LOCTEXT("NarrativeCategory", "Narrative"));
}

#undef LOCTEXT_NAMESPACE
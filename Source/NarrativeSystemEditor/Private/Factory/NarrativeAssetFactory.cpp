// Copyright Maxime Freteau. All Rights Reserved.

#include "Factory/NarrativeAssetFactory.h"
#include "Core/NarrativeAsset.h"
#include "Graph/NarrativeEdGraph.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Graph/NarrativeEdGraphSchema.h"

#define LOCTEXT_NAMESPACE "NarrativeAssetFactory"

UNarrativeAssetFactory::UNarrativeAssetFactory()
{
	SupportedClass = UNarrativeAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UNarrativeAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(UNarrativeAsset::StaticClass()));

	UNarrativeAsset* NewAsset = NewObject<UNarrativeAsset>(InParent, InClass, InName, Flags);

	if (NewAsset)
	{
		UNarrativeEdGraph* NewGraph = NewObject<UNarrativeEdGraph>(NewAsset, UNarrativeEdGraph::StaticClass(), NAME_None, RF_Transactional);
        
		if (NewGraph)
		{
			NewAsset->NarrativeGraph = NewGraph;
            
#if WITH_EDITORONLY_DATA
			NewAsset->EdGraph = NewGraph;
#endif

			NewGraph->NarrativeAsset = NewAsset;
            
			NewGraph->Schema = UNarrativeEdGraphSchema::StaticClass();
            
			const UEdGraphSchema* Schema = NewGraph->GetSchema();
			if (Schema)
			{
				Schema->CreateDefaultNodesForGraph(*NewGraph);
			}
		}
	}

	return NewAsset;
}

FText UNarrativeAssetFactory::GetDisplayName() const
{
	return LOCTEXT("NarrativeAssetFactoryName", "Narrative Asset");
}

uint32 UNarrativeAssetFactory::GetMenuCategories() const
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	return AssetTools.RegisterAdvancedAssetCategory("Narrative", LOCTEXT("NarrativeCategory", "Narrative"));
}

#undef LOCTEXT_NAMESPACE
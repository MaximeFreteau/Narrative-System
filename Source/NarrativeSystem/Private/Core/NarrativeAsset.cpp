// Copyright Maxime Freteau. All Rights Reserved.

#include "Core/NarrativeAsset.h"
#include "Core/NarrativeNode.h"

UNarrativeAsset::UNarrativeAsset()
{
}

UNarrativeNode* UNarrativeAsset::FindNodeByGuid(const FGuid& Guid) const
{
    for (UNarrativeNode* Node : Nodes)
    {
        if (Node && Node->NodeGuid == Guid)
        {
            return Node;
        }
    }
    return nullptr;
}

void UNarrativeAsset::CompileNarrativeGraph()
{
}

void UNarrativeAsset::BuildMetadata()
{
    Metadata.AssetID = GetFName();
    Metadata.Type = Type;
    Metadata.MinLevel = 1;
    Metadata.MaxLevel = 100;
}

#if WITH_EDITOR
void UNarrativeAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName PropertyName = PropertyChangedEvent.GetPropertyName();
    
    if (PropertyName == GET_MEMBER_NAME_CHECKED(UNarrativeAsset, Type) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(UNarrativeAsset, NarrativeName))
    {
        BuildMetadata();
    }
}

void UNarrativeAsset::PostLoad()
{
    Super::PostLoad();
    BuildMetadata();
}
#endif
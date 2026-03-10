// Copyright Maxime Freteau. All Rights Reserved.

#include "Core/NarrativeRegistry.h"
#include "Core/NarrativeAsset.h"
#include "AssetRegistry/AssetRegistryModule.h"

void UNarrativeRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    ScanForNarrativeAssets();
    
    BuildIndices();

}

void UNarrativeRegistry::Deinitialize()
{
    AllNarratives.Empty();
    TypeIndex.Empty();
    LevelIndex.Empty();
    TagIndex.Empty();
    RegionIndex.Empty();
    AssetIDToIndex.Empty();

    Super::Deinitialize();
}

void UNarrativeRegistry::ScanForNarrativeAssets()
{
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    TArray<FAssetData> AssetDataList;
    AssetRegistry.GetAssetsByClass(UNarrativeAsset::StaticClass()->GetClassPathName(), AssetDataList, true);

    for (const FAssetData& AssetData : AssetDataList)
    {
        FNarrativeAssetReference Ref;
        Ref.AssetPtr = TSoftObjectPtr<UNarrativeAsset>(AssetData.GetSoftObjectPath());

        Ref.Metadata.AssetID = AssetData.AssetName;
        
        AllNarratives.Add(Ref);
    }
}

void UNarrativeRegistry::RegisterNarrative(UNarrativeAsset* Asset)
{
    if (!Asset)
    {
        return;
    }

    FName AssetID = Asset->Metadata.AssetID;
    
    if (AssetIDToIndex.Contains(AssetID))
    {
        int32 Index = AssetIDToIndex[AssetID];
        AllNarratives[Index].AssetPtr = Asset;
        AllNarratives[Index].Metadata = Asset->Metadata;
    }
    else
    {
        FNarrativeAssetReference Ref;
        Ref.AssetPtr = Asset;
        Ref.Metadata = Asset->Metadata;
        
        int32 Index = AllNarratives.Add(Ref);
        AssetIDToIndex.Add(AssetID, Index);
    }

    BuildIndices();
}

void UNarrativeRegistry::UnregisterNarrative(FName AssetID)
{
    if (!AssetIDToIndex.Contains(AssetID))
    {
        return;
    }

    int32 Index = AssetIDToIndex[AssetID];
    AllNarratives.RemoveAt(Index);
    AssetIDToIndex.Remove(AssetID);

    BuildIndices();
}

void UNarrativeRegistry::BuildIndices()
{
    TypeIndex.Empty();
    LevelIndex.Empty();
    TagIndex.Empty();
    RegionIndex.Empty();
    AssetIDToIndex.Empty();

    for (int32 i = 0; i < AllNarratives.Num(); ++i)
    {
        const FNarrativeAssetReference& Ref = AllNarratives[i];
        const FNarrativeMetadata& Meta = Ref.Metadata;

        AssetIDToIndex.Add(Meta.AssetID, i);

        TypeIndex.FindOrAdd(Meta.Type).Add(Meta.AssetID);

        for (int32 Level = Meta.MinLevel; Level <= Meta.MaxLevel; ++Level)
        {
            LevelIndex.FindOrAdd(Level).Add(Meta.AssetID);
        }

        for (const FGameplayTag& Tag : Meta.RequiredTags)
        {
            TagIndex.FindOrAdd(Tag).Add(Meta.AssetID);
        }

        if (Meta.RegionTag.IsValid())
        {
            RegionIndex.FindOrAdd(Meta.RegionTag).Add(Meta.AssetID);
        }
    }
}

TArray<FName> UNarrativeRegistry::GetNarrativesByType(ENarrativeType Type) const
{
    const TArray<FName>* Found = TypeIndex.Find(Type);
    return Found ? *Found : TArray<FName>();
}

TArray<FName> UNarrativeRegistry::GetQuestsForLevel(int32 PlayerLevel) const
{
    const TArray<FName>* Found = LevelIndex.Find(PlayerLevel);
    return Found ? *Found : TArray<FName>();
}

TArray<FName> UNarrativeRegistry::GetNarrativesWithTag(FGameplayTag Tag) const
{
    const TArray<FName>* Found = TagIndex.Find(Tag);
    return Found ? *Found : TArray<FName>();
}

TArray<FName> UNarrativeRegistry::GetNarrativesInRegion(FGameplayTag RegionTag) const
{
    const TArray<FName>* Found = RegionIndex.Find(RegionTag);
    return Found ? *Found : TArray<FName>();
}

TArray<FName> UNarrativeRegistry::GetNarrativesInRadius(FVector Center, float Radius) const
{
    TArray<FName> Result;
    float RadiusSquared = Radius * Radius;

    for (const FNarrativeAssetReference& Ref : AllNarratives)
    {
        float DistSquared = FVector::DistSquared(Center, Ref.Metadata.WorldLocation);
        if (DistSquared <= RadiusSquared)
        {
            Result.Add(Ref.Metadata.AssetID);
        }
    }

    return Result;
}

FNarrativeAssetReference* UNarrativeRegistry::FindReference(FName AssetID)
{
    const int32* IndexPtr = AssetIDToIndex.Find(AssetID);
    return IndexPtr ? &AllNarratives[*IndexPtr] : nullptr;
}

const FNarrativeMetadata* UNarrativeRegistry::GetMetadata(FName AssetID) const
{
    const int32* IndexPtr = AssetIDToIndex.Find(AssetID);
    return IndexPtr ? &AllNarratives[*IndexPtr].Metadata : nullptr;
}
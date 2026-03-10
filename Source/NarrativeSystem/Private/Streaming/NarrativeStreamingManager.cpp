// Copyright Maxime Freteau. All Rights Reserved.

#include "Streaming/NarrativeStreamingManager.h"
#include "Core/NarrativeAsset.h"
#include "Core/NarrativeRegistry.h"
#include "Engine/AssetManager.h"

UNarrativeStreamingManager::UNarrativeStreamingManager()
{
    PrimaryComponentTick.bCanEverTick = false;
    MaxCachedAssets = 50;
    bDebugLogging = false;
}

void UNarrativeStreamingManager::BeginPlay()
{
    Super::BeginPlay();

    if (!StreamableManager.IsValid())
    {
        StreamableManager = MakeShared<FStreamableManager>();
    }
}

void UNarrativeStreamingManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto& Pair : ActiveHandles)
    {
        if (Pair.Value.IsValid())
        {
            Pair.Value->CancelHandle();
        }
    }
    ActiveHandles.Empty();

    ClearCache();

    Super::EndPlay(EndPlayReason);
}

void UNarrativeStreamingManager::LoadNarrativeAsync(FName NarrativeID, FOnNarrativeLoaded Callback)
{
    if (UNarrativeAsset* CachedAsset = GetCachedAsset(NarrativeID))
    {
        
        Callback.ExecuteIfBound(CachedAsset);
        return;
    }

    FSoftObjectPath AssetPath = GetAssetPath(NarrativeID);
    if (!AssetPath.IsValid())
    {
        Callback.ExecuteIfBound(nullptr);
        return;
    }

    if (StreamableManager.IsValid())
    {
        TSharedPtr<FStreamableHandle> Handle = StreamableManager->RequestAsyncLoad(
            AssetPath,
            FStreamableDelegate::CreateUObject(this, &UNarrativeStreamingManager::OnAssetLoaded, NarrativeID, Callback)
        );

        if (Handle.IsValid())
        {
            ActiveHandles.Add(NarrativeID, Handle);
        }
    }
}

UNarrativeAsset* UNarrativeStreamingManager::LoadNarrativeSync(FName NarrativeID)
{
    if (UNarrativeAsset* CachedAsset = GetCachedAsset(NarrativeID))
    {
        return CachedAsset;
    }

    FSoftObjectPath AssetPath = GetAssetPath(NarrativeID);
    if (!AssetPath.IsValid())
    {
        return nullptr;
    }

    if (StreamableManager.IsValid())
    {
        UObject* LoadedObject = StreamableManager->LoadSynchronous(AssetPath);
        if (UNarrativeAsset* Asset = Cast<UNarrativeAsset>(LoadedObject))
        {
            LoadedAssets.Add(NarrativeID, Asset);
            UpdateLRU(NarrativeID);
            TrimCache();

            return Asset;
        }
    }

    return nullptr;
}

void UNarrativeStreamingManager::UnloadNarrative(FName NarrativeID)
{

    RemoveFromCache(NarrativeID);

    if (TSharedPtr<FStreamableHandle>* HandlePtr = ActiveHandles.Find(NarrativeID))
    {
        if (HandlePtr->IsValid())
        {
            (*HandlePtr)->CancelHandle();
        }
        ActiveHandles.Remove(NarrativeID);
    }
}

void UNarrativeStreamingManager::OnAssetLoaded(FName AssetID, FOnNarrativeLoaded Callback)
{
    TSharedPtr<FStreamableHandle> Handle;
    if (ActiveHandles.RemoveAndCopyValue(AssetID, Handle))
    {
        if (Handle.IsValid() && Handle->HasLoadCompleted())
        {
            UObject* LoadedObject = Handle->GetLoadedAsset();
            if (UNarrativeAsset* Asset = Cast<UNarrativeAsset>(LoadedObject))
            {
                LoadedAssets.Add(AssetID, Asset);
                UpdateLRU(AssetID);
                TrimCache();

                Callback.ExecuteIfBound(Asset);
                return;
            }
        }
    }

    Callback.ExecuteIfBound(nullptr);
}

UNarrativeAsset* UNarrativeStreamingManager::GetCachedAsset(FName NarrativeID)
{
    if (TObjectPtr<UNarrativeAsset>* AssetPtr = LoadedAssets.Find(NarrativeID))
    {
        UpdateLRU(NarrativeID);
        return *AssetPtr;
    }
    return nullptr;
}

bool UNarrativeStreamingManager::IsAssetLoaded(FName NarrativeID) const
{
    return LoadedAssets.Contains(NarrativeID);
}

void UNarrativeStreamingManager::TrimCache()
{
    while (LoadedAssets.Num() > MaxCachedAssets && LRUQueue.Num() > 0)
    {
        FName LRUAsset = LRUQueue.Last();

        RemoveFromCache(LRUAsset);
    }
}

void UNarrativeStreamingManager::ClearCache()
{

    LoadedAssets.Empty();
    LRUQueue.Empty();
}

void UNarrativeStreamingManager::PreloadAssets(const TArray<FName>& AssetIDs)
{
    for (const FName& AssetID : AssetIDs)
    {
        LoadNarrativeAsync(AssetID, FOnNarrativeLoaded());
    }
}

TArray<FName> UNarrativeStreamingManager::GetCachedAssetIDs() const
{
    TArray<FName> Result;
    LoadedAssets.GetKeys(Result);
    return Result;
}

void UNarrativeStreamingManager::UpdateLRU(FName AssetID)
{
    LRUQueue.Remove(AssetID);
    
    LRUQueue.Insert(AssetID, 0);
}

void UNarrativeStreamingManager::RemoveFromCache(FName AssetID)
{
    LoadedAssets.Remove(AssetID);
    LRUQueue.Remove(AssetID);
}

FSoftObjectPath UNarrativeStreamingManager::GetAssetPath(FName AssetID) const
{
    UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
    if (!GameInstance)
    {
        return FSoftObjectPath();
    }

    UNarrativeRegistry* Registry = GameInstance->GetSubsystem<UNarrativeRegistry>();
    if (!Registry)
    {
        return FSoftObjectPath();
    }

    FNarrativeAssetReference* Ref = Registry->FindReference(AssetID);
    if (!Ref)
    {
        return FSoftObjectPath();
    }

    return Ref->AssetPtr.ToSoftObjectPath();
}
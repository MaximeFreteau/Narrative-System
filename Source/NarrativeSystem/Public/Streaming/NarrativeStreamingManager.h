// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StreamableManager.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeStreamingManager.generated.h"

class UNarrativeAsset;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnNarrativeLoaded, UNarrativeAsset*, LoadedAsset);

UCLASS(ClassGroup=(Narrative), meta=(BlueprintSpawnableComponent))
class NARRATIVESYSTEM_API UNarrativeStreamingManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UNarrativeStreamingManager();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    void LoadNarrativeAsync(FName NarrativeID, FOnNarrativeLoaded Callback);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    void UnloadNarrative(FName NarrativeID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    UNarrativeAsset* LoadNarrativeSync(FName NarrativeID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    UNarrativeAsset* GetCachedAsset(FName NarrativeID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    bool IsAssetLoaded(FName NarrativeID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    void TrimCache();

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    void PreloadAssets(const TArray<FName>& AssetIDs);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    int32 MaxCachedAssets = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugLogging = false;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    int32 GetCachedAssetsCount() const { return LoadedAssets.Num(); }

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    TArray<FName> GetCachedAssetIDs() const;

protected:
    UPROPERTY()
    TMap<FName, TObjectPtr<UNarrativeAsset>> LoadedAssets;

    TArray<FName> LRUQueue;

    TSharedPtr<FStreamableManager> StreamableManager;

    TMap<FName, TSharedPtr<FStreamableHandle>> ActiveHandles;

private:
    void OnAssetLoaded(FName AssetID, FOnNarrativeLoaded Callback);
    void UpdateLRU(FName AssetID);
    void RemoveFromCache(FName AssetID);
    FSoftObjectPath GetAssetPath(FName AssetID) const;
};
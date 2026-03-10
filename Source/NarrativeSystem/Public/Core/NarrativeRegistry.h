// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NarrativeTypes.h"
#include "NarrativeRegistry.generated.h"

class UNarrativeAsset;

UCLASS()
class NARRATIVESYSTEM_API UNarrativeRegistry : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Registry")
    void RegisterNarrative(UNarrativeAsset* Asset);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Registry")
    void UnregisterNarrative(FName AssetID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Registry")
    TArray<FName> GetNarrativesByType(ENarrativeType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Registry")
    TArray<FName> GetQuestsForLevel(int32 PlayerLevel) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Registry")
    TArray<FName> GetNarrativesWithTag(FGameplayTag Tag) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Registry")
    TArray<FName> GetNarrativesInRegion(FGameplayTag RegionTag) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Registry")
    TArray<FName> GetNarrativesInRadius(FVector Center, float Radius) const;

    FNarrativeAssetReference* FindReference(FName AssetID);

    const FNarrativeMetadata* GetMetadata(FName AssetID) const;

protected:
    UPROPERTY()
    TArray<FNarrativeAssetReference> AllNarratives;

    TMap<ENarrativeType, TArray<FName>> TypeIndex;
    TMap<int32, TArray<FName>> LevelIndex;
    TMap<FGameplayTag, TArray<FName>> TagIndex;
    TMap<FGameplayTag, TArray<FName>> RegionIndex;

    TMap<FName, int32> AssetIDToIndex;

private:
    void BuildIndices();
    void ScanForNarrativeAssets();
};
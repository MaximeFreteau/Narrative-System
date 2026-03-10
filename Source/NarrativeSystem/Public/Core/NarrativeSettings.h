// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NarrativeSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Narrative System"))
class NARRATIVESYSTEM_API UNarrativeSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UNarrativeSettings();

    UPROPERTY(Config, EditAnywhere, Category = "Streaming")
    bool bEnableProximityStreaming = true;

    UPROPERTY(Config, EditAnywhere, Category = "Streaming", meta = (EditCondition = "bEnableProximityStreaming"))
    float StreamingRadius = 200000.f;

    UPROPERTY(Config, EditAnywhere, Category = "Streaming", meta = (EditCondition = "bEnableProximityStreaming"))
    float StreamingInterval = 2.f;

    UPROPERTY(Config, EditAnywhere, Category = "Streaming")
    int32 MaxCachedAssets = 50;

    UPROPERTY(Config, EditAnywhere, Category = "Spatial")
    bool bEnableSpatialIndex = true;

    UPROPERTY(Config, EditAnywhere, Category = "Spatial", meta = (EditCondition = "bEnableSpatialIndex"))
    float CellSize = 100000.f;

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    bool bUseDeltaCompression = true;

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    bool bUseIncrementalSaves = true;

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    int32 MaxDialogueHistory = 100;

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    float AutoSaveInterval = 300.f;

    UPROPERTY(Config, EditAnywhere, Category = "Performance")
    bool bUseAsyncLoading = true;

    UPROPERTY(Config, EditAnywhere, Category = "Performance")
    bool bPreloadCriticalNarratives = false;

    UPROPERTY(Config, EditAnywhere, Category = "Performance")
    int32 MaxConcurrentLoads = 3;

    UPROPERTY(Config, EditAnywhere, Category = "Debug")
    bool bEnableDebugLogging = false;

    UPROPERTY(Config, EditAnywhere, Category = "Debug")
    bool bShowStreamingDebug = false;

    virtual FName GetCategoryName() const override { return TEXT("Game"); }
#if WITH_EDITOR
    virtual FText GetSectionText() const override { return FText::FromString(TEXT("Narrative System")); }
#endif
};
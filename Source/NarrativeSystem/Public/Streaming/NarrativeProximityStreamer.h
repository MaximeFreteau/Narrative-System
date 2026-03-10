// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NarrativeProximityStreamer.generated.h"

class UNarrativeStreamingManager;

UCLASS(ClassGroup=(Narrative), meta=(BlueprintSpawnableComponent))
class NARRATIVESYSTEM_API UNarrativeProximityStreamer : public UActorComponent
{
    GENERATED_BODY()

public:
    UNarrativeProximityStreamer();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    float StreamingRadius = 200000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    float StreamInterval = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Streaming")
    bool bEnableAutoStreaming = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugLogging = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bShowDebugRadius = false;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    void ForceUpdate();

    UFUNCTION(BlueprintCallable, Category = "Narrative|Streaming")
    void SetStreamingEnabled(bool bEnabled);

protected:

    UPROPERTY()
    TObjectPtr<UNarrativeStreamingManager> StreamingManager;

    float StreamTimer = 0.f;

    FVector LastPlayerLocation = FVector::ZeroVector;

    TSet<FName> StreamedAssets;

private:
    void UpdateNearbyNarratives();
    void UnloadDistantNarratives(FVector PlayerLocation);
    void LoadNearbyNarratives(FVector PlayerLocation);
    FVector GetPlayerLocation() const;
};
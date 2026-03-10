// Copyright Maxime Freteau. All Rights Reserved.

#include "Streaming/NarrativeProximityStreamer.h"
#include "Streaming/NarrativeStreamingManager.h"
#include "Core/NarrativeRegistry.h"
#include "Core/NarrativeSettings.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UNarrativeProximityStreamer::UNarrativeProximityStreamer()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    const UNarrativeSettings* Settings = GetDefault<UNarrativeSettings>();
    if (Settings)
    {
        StreamingRadius = Settings->StreamingRadius;
        StreamInterval = Settings->StreamingInterval;
        bEnableAutoStreaming = Settings->bEnableProximityStreaming;
        bDebugLogging = Settings->bEnableDebugLogging;
        bShowDebugRadius = Settings->bShowStreamingDebug;
    }
}

void UNarrativeProximityStreamer::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        StreamingManager = Owner->FindComponentByClass<UNarrativeStreamingManager>();
    }

    if (bEnableAutoStreaming)
    {
        ForceUpdate();
    }
}

void UNarrativeProximityStreamer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bEnableAutoStreaming || !StreamingManager)
    {
        return;
    }

    StreamTimer += DeltaTime;

    if (StreamTimer >= StreamInterval)
    {
        StreamTimer = 0.f;
        UpdateNearbyNarratives();
    }

    if (bShowDebugRadius)
    {
        FVector PlayerLocation = GetPlayerLocation();
        DrawDebugSphere(GetWorld(), PlayerLocation, StreamingRadius, 32, FColor::Green, false, StreamInterval);
    }
}

void UNarrativeProximityStreamer::ForceUpdate()
{
    if (StreamingManager)
    {
        UpdateNearbyNarratives();
    }
}

void UNarrativeProximityStreamer::SetStreamingEnabled(bool bEnabled)
{
    bEnableAutoStreaming = bEnabled;
    
    if (bEnabled)
    {
        ForceUpdate();
    }
}

void UNarrativeProximityStreamer::UpdateNearbyNarratives()
{
    FVector PlayerLocation = GetPlayerLocation();
    
    float DistanceMoved = FVector::Dist(PlayerLocation, LastPlayerLocation);
    if (DistanceMoved < StreamingRadius * 0.1f)
    {
        return;
    }

    LastPlayerLocation = PlayerLocation;

    UnloadDistantNarratives(PlayerLocation);

    LoadNearbyNarratives(PlayerLocation);
}

void UNarrativeProximityStreamer::UnloadDistantNarratives(FVector PlayerLocation)
{
    UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
    if (!GameInstance)
    {
        return;
    }

    UNarrativeRegistry* Registry = GameInstance->GetSubsystem<UNarrativeRegistry>();
    if (!Registry)
    {
        return;
    }

    TArray<FName> ToUnload;

    for (const FName& AssetID : StreamedAssets)
    {
        const FNarrativeMetadata* Metadata = Registry->GetMetadata(AssetID);
        if (!Metadata)
        {
            continue;
        }

        float Distance = FVector::Dist(PlayerLocation, Metadata->WorldLocation);
        if (Distance > StreamingRadius * 1.2f)
        {
            ToUnload.Add(AssetID);
        }
    }

    for (const FName& AssetID : ToUnload)
    {

        StreamingManager->UnloadNarrative(AssetID);
        StreamedAssets.Remove(AssetID);
    }
}

void UNarrativeProximityStreamer::LoadNearbyNarratives(FVector PlayerLocation)
{
    UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
    if (!GameInstance)
    {
        return;
    }

    UNarrativeRegistry* Registry = GameInstance->GetSubsystem<UNarrativeRegistry>();
    if (!Registry)
    {
        return;
    }

    TArray<FName> NearbyNarratives = Registry->GetNarrativesInRadius(PlayerLocation, StreamingRadius);

    for (const FName& AssetID : NearbyNarratives)
    {
        if (!StreamedAssets.Contains(AssetID))
        {
            StreamingManager->LoadNarrativeAsync(AssetID, FOnNarrativeLoaded());
            StreamedAssets.Add(AssetID);
        }
    }
}

FVector UNarrativeProximityStreamer::GetPlayerLocation() const
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && PC->GetPawn())
    {
        return PC->GetPawn()->GetActorLocation();
    }
    return FVector::ZeroVector;
}
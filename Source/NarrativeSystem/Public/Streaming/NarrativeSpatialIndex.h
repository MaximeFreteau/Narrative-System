// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NarrativeSpatialIndex.generated.h"

USTRUCT()
struct FNarrativeCell
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FName> NarrativeIDs;
};

UCLASS()
class NARRATIVESYSTEM_API UNarrativeSpatialIndex : public UObject
{
    GENERATED_BODY()

public:
    UNarrativeSpatialIndex();

    UPROPERTY()
    float CellSize = 100000.f;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    void RegisterNarrative(FName NarrativeID, FVector WorldLocation);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    void UnregisterNarrative(FName NarrativeID, FVector WorldLocation);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    void ClearIndex();

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    TArray<FName> GetNarrativesInRadius(FVector Center, float Radius) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    TArray<FName> GetNarrativesInCell(FIntVector CellCoord) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    TArray<FIntVector> GetCellsInRadius(FVector Center, float Radius) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    FIntVector WorldToCellCoord(FVector WorldLocation) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    FVector CellCoordToWorld(FIntVector CellCoord) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    int32 GetCellCount() const { return Grid.Num(); }

    UFUNCTION(BlueprintCallable, Category = "Narrative|Spatial")
    int32 GetTotalNarrativeCount() const;

protected:
    UPROPERTY()
    TMap<FIntVector, FNarrativeCell> Grid;

    UPROPERTY()
    TMap<FName, FIntVector> NarrativeToCell;
};
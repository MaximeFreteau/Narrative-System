// Copyright Maxime Freteau. All Rights Reserved.

#include "Streaming/NarrativeSpatialIndex.h"
#include "Core/NarrativeSettings.h"

UNarrativeSpatialIndex::UNarrativeSpatialIndex()
{
    const UNarrativeSettings* Settings = GetDefault<UNarrativeSettings>();
    if (Settings)
    {
        CellSize = Settings->CellSize;
    }
}

void UNarrativeSpatialIndex::RegisterNarrative(FName NarrativeID, FVector WorldLocation)
{
    FIntVector CellCoord = WorldToCellCoord(WorldLocation);

    FNarrativeCell& Cell = Grid.FindOrAdd(CellCoord);
    Cell.NarrativeIDs.AddUnique(NarrativeID);

    NarrativeToCell.Add(NarrativeID, CellCoord);
}

void UNarrativeSpatialIndex::UnregisterNarrative(FName NarrativeID, FVector WorldLocation)
{
    FIntVector* CellCoordPtr = NarrativeToCell.Find(NarrativeID);
    if (!CellCoordPtr)
    {
        return;
    }

    FIntVector CellCoord = *CellCoordPtr;

    if (FNarrativeCell* Cell = Grid.Find(CellCoord))
    {
        Cell->NarrativeIDs.Remove(NarrativeID);

        if (Cell->NarrativeIDs.Num() == 0)
        {
            Grid.Remove(CellCoord);
        }
    }

    NarrativeToCell.Remove(NarrativeID);
}

void UNarrativeSpatialIndex::ClearIndex()
{
    Grid.Empty();
    NarrativeToCell.Empty();
}

TArray<FName> UNarrativeSpatialIndex::GetNarrativesInRadius(FVector Center, float Radius) const
{
    TSet<FName> ResultSet;
    float RadiusSquared = Radius * Radius;

    TArray<FIntVector> CellsInRadius = GetCellsInRadius(Center, Radius);

    for (const FIntVector& CellCoord : CellsInRadius)
    {
        const FNarrativeCell* Cell = Grid.Find(CellCoord);
        if (!Cell)
        {
            continue;
        }

        for (const FName& NarrativeID : Cell->NarrativeIDs)
        {
            ResultSet.Add(NarrativeID);
        }
    }

    return ResultSet.Array();
}

TArray<FName> UNarrativeSpatialIndex::GetNarrativesInCell(FIntVector CellCoord) const
{
    const FNarrativeCell* Cell = Grid.Find(CellCoord);
    return Cell ? Cell->NarrativeIDs : TArray<FName>();
}

TArray<FIntVector> UNarrativeSpatialIndex::GetCellsInRadius(FVector Center, float Radius) const
{
    TArray<FIntVector> Result;

    FIntVector CenterCell = WorldToCellCoord(Center);

    int32 CellRadius = FMath::CeilToInt(Radius / CellSize);

    for (int32 X = -CellRadius; X <= CellRadius; ++X)
    {
        for (int32 Y = -CellRadius; Y <= CellRadius; ++Y)
        {
            for (int32 Z = -CellRadius; Z <= CellRadius; ++Z)
            {
                FIntVector CellCoord = CenterCell + FIntVector(X, Y, Z);

                if (Grid.Contains(CellCoord))
                {
                    FVector CellCenter = CellCoordToWorld(CellCoord);
                    float DistSquared = FVector::DistSquared(Center, CellCenter);
                    float CheckRadius = Radius + (CellSize * 0.866f);
                    
                    if (DistSquared <= (CheckRadius * CheckRadius))
                    {
                        Result.Add(CellCoord);
                    }
                }
            }
        }
    }

    return Result;
}

int32 UNarrativeSpatialIndex::GetTotalNarrativeCount() const
{
    int32 Count = 0;
    for (const auto& Pair : Grid)
    {
        Count += Pair.Value.NarrativeIDs.Num();
    }
    return Count;
}

FIntVector UNarrativeSpatialIndex::WorldToCellCoord(FVector WorldLocation) const
{
    return FIntVector(
        FMath::FloorToInt(WorldLocation.X / CellSize),
        FMath::FloorToInt(WorldLocation.Y / CellSize),
        FMath::FloorToInt(WorldLocation.Z / CellSize)
    );
}

FVector UNarrativeSpatialIndex::CellCoordToWorld(FIntVector CellCoord) const
{
    return FVector(
        (CellCoord.X + 0.5f) * CellSize,
        (CellCoord.Y + 0.5f) * CellSize,
        (CellCoord.Z + 0.5f) * CellSize
    );
}
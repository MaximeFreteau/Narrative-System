// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Random.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Random::UNarrativeNode_Random()
{
    NodeTitle = FText::FromString("Random");
    bRememberChoice = false;
    LastSelectedIndex = -1;

    FRandomOutput Output1;
    Output1.Weight = 1.f;
    Output1.Description = FText::FromString("Option 1");
    OutputWeights.Add(Output1);

    FRandomOutput Output2;
    Output2.Weight = 1.f;
    Output2.Description = FText::FromString("Option 2");
    OutputWeights.Add(Output2);
}

UNarrativeNode* UNarrativeNode_Random::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return nullptr;
    }

    if (bRememberChoice && LastSelectedIndex >= 0)
    {
        return GetOutputNode(LastSelectedIndex);
    }

    int32 SelectedIndex = SelectRandomOutput();
    
    if (bRememberChoice)
    {
        LastSelectedIndex = SelectedIndex;
    }

    return GetOutputNode(SelectedIndex);
}

int32 UNarrativeNode_Random::SelectRandomOutput() const
{
    if (OutputWeights.Num() == 0)
    {
        return 0;
    }

    float TotalWeight = 0.f;
    for (const FRandomOutput& Output : OutputWeights)
    {
        TotalWeight += Output.Weight;
    }

    if (TotalWeight <= 0.f)
    {
        return FMath::RandRange(0, OutputWeights.Num() - 1);
    }

    float RandomValue = FMath::FRandRange(0.f, TotalWeight);

    float CurrentWeight = 0.f;
    for (int32 i = 0; i < OutputWeights.Num(); ++i)
    {
        CurrentWeight += OutputWeights[i].Weight;
        if (RandomValue <= CurrentWeight)
        {
            return i;
        }
    }

    return OutputWeights.Num() - 1;
}

#if WITH_EDITOR
FText UNarrativeNode_Random::GetNodeTitle() const
{
    return FText::FromString("Random");
}

FLinearColor UNarrativeNode_Random::GetNodeTitleColor() const
{
    return FLinearColor(0.6f, 0.4f, 0.8f);
}

FText UNarrativeNode_Random::GetNodeTooltip() const
{
    FString TooltipText = TEXT("Random Node\n\n");
    TooltipText += FString::Printf(TEXT("Outputs: %d\n"), OutputWeights.Num());
    
    if (bRememberChoice)
    {
        TooltipText += TEXT("Remembers choice\n");
    }

    TooltipText += TEXT("\nWeights:\n");
    for (int32 i = 0; i < OutputWeights.Num(); ++i)
    {
        TooltipText += FString::Printf(TEXT("  %d: %.2f"), i, OutputWeights[i].Weight);
        if (!OutputWeights[i].Description.IsEmpty())
        {
            TooltipText += FString::Printf(TEXT(" (%s)"), *OutputWeights[i].Description.ToString());
        }
        TooltipText += TEXT("\n");
    }

    return FText::FromString(TooltipText);
}
#endif
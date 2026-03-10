// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Hub.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Hub::UNarrativeNode_Hub()
{
    NodeTitle = FText::FromString("Hub");
    
    HubID = FName(*FString::Printf(TEXT("Hub_%d"), FMath::Rand()));
    
    HubDisplayName = FText::FromString("Hub");
    HubDescription = FText::GetEmpty();
    NumInputPins = 3;
    NumOutputPins = 3;
    bIsSavePoint = false;
    bResetStateOnEnter = false;
    bAllowMultipleVisits = true;
    
    VisitCount = 0;
    LastInputPin = -1;
}

UNarrativeNode* UNarrativeNode_Hub::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return nullptr;
    }

    VisitCount++;

    if (!bAllowMultipleVisits && VisitCount > 1)
    {
        return nullptr;
    }

    if (bResetStateOnEnter)
    {
        ResetHub();
    }

    TArray<UNarrativeNode*> ValidOutputs = GetValidOutputs(NarrativeComp);

    if (ValidOutputs.Num() == 0)
    {
        return nullptr;
    }

    return ValidOutputs[0];
}

TArray<UNarrativeNode*> UNarrativeNode_Hub::GetValidOutputs(UNarrativeComponent* NarrativeComp) const
{
    TArray<UNarrativeNode*> ValidOutputs;

    if (!NarrativeComp)
    {
        return ValidOutputs;
    }

    for (int32 i = 0; i < NumOutputPins; i++)
    {
        UNarrativeNode* OutputNode = GetOutputNode(i);
        if (OutputNode)
        {
            ValidOutputs.Add(OutputNode);
        }
    }

    return ValidOutputs;
}

void UNarrativeNode_Hub::RegisterInputVisit(int32 InputIndex)
{
    if (InputIndex >= 0 && InputIndex < NumInputPins)
    {
        VisitedInputs.Add(InputIndex);
        LastInputPin = InputIndex;
    }
}

bool UNarrativeNode_Hub::AreAllInputsVisited() const
{
    return VisitedInputs.Num() >= NumInputPins;
}

void UNarrativeNode_Hub::ResetHub()
{
    VisitCount = 0;
    LastInputPin = -1;
    VisitedInputs.Empty();
}

#if WITH_EDITOR

FText UNarrativeNode_Hub::GetNodeTitle() const
{
    if (!HubDisplayName.IsEmpty())
    {
        return FText::Format(FText::FromString("Hub: {0}"), HubDisplayName);
    }
    return FText::FromString("Hub");
}

FLinearColor UNarrativeNode_Hub::GetNodeTitleColor() const
{
    return FLinearColor(0.0f, 0.8f, 0.8f);
}

FText UNarrativeNode_Hub::GetNodeTooltip() const
{
    FString Tooltip = FString::Printf(TEXT("Hub: %s\n"), *HubDisplayName.ToString());
    
    if (!HubDescription.IsEmpty())
    {
        Tooltip += FString::Printf(TEXT("%s\n\n"), *HubDescription.ToString());
    }
    
    Tooltip += FString::Printf(TEXT("Inputs: %d\n"), NumInputPins);
    Tooltip += FString::Printf(TEXT("Outputs: %d\n"), NumOutputPins);
    
    if (bIsSavePoint)
    {
        Tooltip += TEXT("(Save Point) ");
    }
    if (bResetStateOnEnter)
    {
        Tooltip += TEXT("(Resets State) ");
    }
    if (!bAllowMultipleVisits)
    {
        Tooltip += TEXT("(Single Visit) ");
    }
    
    return FText::FromString(Tooltip);
}

void UNarrativeNode_Hub::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    if (HubID == NAME_None)
    {
        HubID = FName(*FString::Printf(TEXT("Hub_%d"), FMath::Rand()));
    }

    NumInputPins = FMath::Clamp(NumInputPins, 1, 10);
    NumOutputPins = FMath::Clamp(NumOutputPins, 1, 10);
}

#endif

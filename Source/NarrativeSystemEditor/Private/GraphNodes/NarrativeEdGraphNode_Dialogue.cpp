// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Dialogue.h"
#include "Nodes/NarrativeNode_Dialogue.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Dialogue::UNarrativeEdGraphNode_Dialogue()
{
}

void UNarrativeEdGraphNode_Dialogue::CreateRuntimeNode()
{
    UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
    if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
    {
        return;
    }

    RuntimeNode = NewObject<UNarrativeNode_Dialogue>(NarrativeGraph->NarrativeAsset);
    RuntimeNode->GraphNode = this;
}

int32 UNarrativeEdGraphNode_Dialogue::GetOutputPinCount() const
{
    if (UNarrativeNode_Dialogue* DialogueNode = Cast<UNarrativeNode_Dialogue>(RuntimeNode))
    {
        return FMath::Max(1, DialogueNode->Choices.Num());
    }
    return 1;
}

void UNarrativeEdGraphNode_Dialogue::AllocateDefaultPins()
{
    if (!RuntimeNode)
    {
        CreateRuntimeNode();
    }

    FCreatePinParams InputParams;
    InputParams.Index = 0;
    UEdGraphNode::CreatePin(EGPD_Input, TEXT("Narrative"), TEXT("In"), InputParams);

    UNarrativeNode_Dialogue* DialogueNode = Cast<UNarrativeNode_Dialogue>(RuntimeNode);
    
    if (DialogueNode && DialogueNode->Choices.Num() > 0)
    {
        for (int32 i = 0; i < DialogueNode->Choices.Num(); ++i)
        {
            FCreatePinParams OutputParams;
            OutputParams.Index = i;
            
            FName PinName = *FString::Printf(TEXT("Choice_%d"), i);
            UEdGraphPin* Pin = UEdGraphNode::CreatePin(EGPD_Output, TEXT("Narrative"), PinName, OutputParams);
            
            if (Pin)
            {
                if (!DialogueNode->Choices[i].ChoiceText.IsEmpty())
                {
                    Pin->PinFriendlyName = DialogueNode->Choices[i].ChoiceText;
                }
                else
                {
                    Pin->PinFriendlyName = FText::FromString(FString::Printf(TEXT("Choice %d"), i));
                }
            }
        }
    }
    else
    {
        FCreatePinParams OutputParams;
        OutputParams.Index = 0;
        UEdGraphNode::CreatePin(EGPD_Output, TEXT("Narrative"), TEXT("Out"), OutputParams);
    }
}

void UNarrativeEdGraphNode_Dialogue::RefreshPins()
{
    ReconstructNode();
}
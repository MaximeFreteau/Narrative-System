// Copyright Maxime Freteau. All Rights Reserved.

#include "Graph/NarrativeEdGraphNode.h"
#include "Core/NarrativeNode.h"
#include "Graph/NarrativeEdGraph.h"
#include "ToolMenus.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "NarrativeEdGraphNode"

UNarrativeEdGraphNode::UNarrativeEdGraphNode()
{
}

void UNarrativeEdGraphNode::AllocateDefaultPins()
{
    CreateInputPins();
    CreateOutputPins();
}

void UNarrativeEdGraphNode::CreateInputPins()
{
    const int32 InputCount = GetInputPinCount();
    for (int32 i = 0; i < InputCount; ++i)
    {
        FCreatePinParams PinParams;
        PinParams.Index = i;
        UEdGraphNode::CreatePin(EGPD_Input, TEXT("Narrative"), FName(*FString::Printf(TEXT("In_%d"), i)), PinParams);
    }
}

void UNarrativeEdGraphNode::CreateOutputPins()
{
    const int32 OutputCount = GetOutputPinCount();
    for (int32 i = 0; i < OutputCount; ++i)
    {
        FCreatePinParams PinParams;
        PinParams.Index = i;
        UEdGraphNode::CreatePin(EGPD_Output, TEXT("Narrative"), FName(*FString::Printf(TEXT("Out_%d"), i)), PinParams);
    }
}

FText UNarrativeEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (RuntimeNode)
    {
        return RuntimeNode->GetNodeTitle();
    }
    return LOCTEXT("DefaultNodeTitle", "Narrative Node");
}

FLinearColor UNarrativeEdGraphNode::GetNodeTitleColor() const
{
    if (RuntimeNode)
    {
        return RuntimeNode->GetNodeTitleColor();
    }
    return FLinearColor::White;
}

FText UNarrativeEdGraphNode::GetTooltipText() const
{
    if (RuntimeNode)
    {
        return RuntimeNode->GetNodeTooltip();
    }
    return LOCTEXT("DefaultTooltip", "Narrative Node");
}

bool UNarrativeEdGraphNode::CanUserDeleteNode() const
{
    if (IsRootNode())
    {
        return false;
    }

    if (RuntimeNode)
    {
        return RuntimeNode->CanUserDeleteNode();
    }
    return true;
}

void UNarrativeEdGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
    if (FromPin)
    {
        for (UEdGraphPin* Pin : Pins)
        {
            if (Pin->Direction == EGPD_Input && GetSchema()->TryCreateConnection(FromPin, Pin))
            {
                break;
            }
        }
    }

    if (UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph()))
    {
        NarrativeGraph->NotifyGraphChanged();
    }
}

void UNarrativeEdGraphNode::PrepareForCopying()
{
    if (RuntimeNode)
    {
        RuntimeNode->Rename(nullptr, this, REN_DontCreateRedirectors);
    }
}

void UNarrativeEdGraphNode::PostPasteNode()
{
    Super::PostPasteNode();

    if (RuntimeNode)
    {
        RuntimeNode->NodeGuid = FGuid::NewGuid();
    }
}

void UNarrativeEdGraphNode::DestroyNode()
{
    if (RuntimeNode)
    {
        RuntimeNode->ConditionalBeginDestroy();
        RuntimeNode = nullptr;
    }

    Super::DestroyNode();
}

void UNarrativeEdGraphNode::CreateRuntimeNode()
{
    ensureMsgf(false, TEXT("CreateRuntimeNode() must be overridden in derived classes!"));
}

void UNarrativeEdGraphNode::ReconstructNode()
{
    if (GIsTransacting || !GetGraph())
    {
        return;
    }

    Modify();

    for (UEdGraphPin* Pin : Pins)
    {
        if (Pin)
        {
            Pin->BreakAllPinLinks();
        }
    }

    Pins.Empty();
    AllocateDefaultPins();
}

void UNarrativeEdGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    if (!Menu || !Context)
    {
        return;
    }

    FToolMenuSection& Section = Menu->AddSection("NarrativeNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));

    Section.AddMenuEntry(
        "RefreshPins",
        LOCTEXT("RefreshPins", "Refresh Pins"),
        LOCTEXT("RefreshPinsTooltip", "Rebuild pins (will disconnect all connections)"),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda([this]()
            {
                UNarrativeEdGraphNode* MutableNode = const_cast<UNarrativeEdGraphNode*>(this);
                
                if (MutableNode && MutableNode->GetGraph())
                {
                    const FScopedTransaction Transaction(LOCTEXT("RefreshPins", "Refresh Pins"));
                    
                    for (UEdGraphPin* Pin : MutableNode->Pins)
                    {
                        if (Pin)
                        {
                            Pin->BreakAllPinLinks();
                        }
                    }
                    
                    MutableNode->Pins.Empty();
                    
                    MutableNode->AllocateDefaultPins();
                    
                    MutableNode->GetGraph()->NotifyGraphChanged();
                }
            })
        )
    );
}

#if WITH_EDITOR
void UNarrativeEdGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.GetPropertyName() == NAME_None)
    {
        return;
    }

    if (GIsTransacting)
    {
        return;
    }

    if (!GetGraph())
    {
        return;
    }

    ReconstructNode();
}
#endif

#undef LOCTEXT_NAMESPACE
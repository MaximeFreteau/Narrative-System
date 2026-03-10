// Copyright Maxime Freteau. All Rights Reserved.

#include "Graph/NarrativeEdGraphSchema.h"
#include "Graph/NarrativeEdGraph.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "Core/NarrativeNode.h"
#include "Nodes/NarrativeNode_Dialogue.h"
#include "Nodes/NarrativeNode_Hub.h"
#include "Nodes/NarrativeNode_Quest.h"
#include "Nodes/NarrativeNode_Objective.h"
#include "Nodes/NarrativeNode_Condition.h"
#include "Nodes/NarrativeNode_Branch.h"
#include "Nodes/NarrativeNode_Event.h"
#include "Nodes/NarrativeNode_Reward.h"
#include "Nodes/NarrativeNode_SetVariable.h"
#include "Nodes/NarrativeNode_Random.h"
#include "GraphNodes/NarrativeEdGraphNode_Dialogue.h"
#include "GraphNodes/NarrativeEdGraphNode_Hub.h"
#include "GraphNodes/NarrativeEdGraphNode_Quest.h"
#include "GraphNodes/NarrativeEdGraphNode_Objective.h"
#include "GraphNodes/NarrativeEdGraphNode_Condition.h"
#include "GraphNodes/NarrativeEdGraphNode_Branch.h"
#include "GraphNodes/NarrativeEdGraphNode_Event.h"
#include "GraphNodes/NarrativeEdGraphNode_Reward.h"
#include "GraphNodes/NarrativeEdGraphNode_SetVariable.h"
#include "GraphNodes/NarrativeEdGraphNode_Random.h"
#include "EdGraphNode_Comment.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "NarrativeEdGraphSchema"

void UNarrativeEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    GetNarrativeNodeActions(ContextMenuBuilder);
    GetCommentAction(ContextMenuBuilder);
}

void UNarrativeEdGraphSchema::GetNarrativeNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("DialogueCategory", "Dialogue"),
            LOCTEXT("DialogueNodeName", "Dialogue"),
            LOCTEXT("DialogueNodeTooltip", "Create a dialogue node with multi-speaker support"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Dialogue::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("DialogueCategory", "Dialogue"),
            LOCTEXT("HubNodeName", "Hub (NPC Entry Point)"),
            LOCTEXT("HubNodeTooltip", "Create a hub node for recurring NPCs"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Hub::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("QuestCategory", "Quest"),
            LOCTEXT("QuestNodeName", "Quest"),
            LOCTEXT("QuestNodeTooltip", "Create a quest node"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Quest::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("QuestCategory", "Quest"),
            LOCTEXT("ObjectiveNodeName", "Objective"),
            LOCTEXT("ObjectiveNodeTooltip", "Create a quest objective"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Objective::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("FlowCategory", "Flow Control"),
            LOCTEXT("ConditionNodeName", "Condition"),
            LOCTEXT("ConditionNodeTooltip", "Evaluate a condition"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Condition::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("FlowCategory", "Flow Control"),
            LOCTEXT("BranchNodeName", "Branch"),
            LOCTEXT("BranchNodeTooltip", "Branch based on a condition (True/False outputs)"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Branch::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("FlowCategory", "Flow Control"),
            LOCTEXT("RandomNodeName", "Random"),
            LOCTEXT("RandomNodeTooltip", "Randomly select an output with weighted probabilities"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Random::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("ActionsCategory", "Actions"),
            LOCTEXT("EventNodeName", "Event"),
            LOCTEXT("EventNodeTooltip", "Trigger a gameplay event"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Event::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("ActionsCategory", "Actions"),
            LOCTEXT("RewardNodeName", "Reward"),
            LOCTEXT("RewardNodeTooltip", "Give rewards (XP, gold, items)"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_Reward::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }

    {
        TSharedPtr<FNarrativeGraphSchemaAction_NewNode> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewNode>(
            LOCTEXT("ActionsCategory", "Actions"),
            LOCTEXT("SetVariableNodeName", "Set Variable"),
            LOCTEXT("SetVariableNodeTooltip", "Modify a narrative variable"),
            0
        );
        NewAction->NodeClass = UNarrativeNode_SetVariable::StaticClass();
        ContextMenuBuilder.AddAction(NewAction);
    }
}

void UNarrativeEdGraphSchema::GetCommentAction(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    TSharedPtr<FNarrativeGraphSchemaAction_NewComment> NewAction = MakeShared<FNarrativeGraphSchemaAction_NewComment>(
        LOCTEXT("CommentCategory", "Misc"),
        LOCTEXT("CommentNodeName", "Add Comment..."),
        LOCTEXT("CommentNodeTooltip", "Create a comment box"),
        0
    );
    ContextMenuBuilder.AddAction(NewAction);
}

const FPinConnectionResponse UNarrativeEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    if (!A || !B)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid pins"));
    }

    if (A == B)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect a pin to itself"));
    }

    if (A->GetOwningNode() == B->GetOwningNode())
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect pins on the same node"));
    }

    if (A->Direction == B->Direction)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect input to input or output to output"));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool UNarrativeEdGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
    const FPinConnectionResponse Response = CanCreateConnection(A, B);
    
    if (Response.Response == CONNECT_RESPONSE_MAKE)
    {
        A->MakeLinkTo(B);
        
        if (UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(A->GetOwningNode()->GetGraph()))
        {
            NarrativeGraph->NotifyGraphChanged();
        }
        
        return true;
    }

    return false;
}

void UNarrativeEdGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
    FGraphNodeCreator<UNarrativeEdGraphNode_Hub> NodeCreator(Graph);
    UNarrativeEdGraphNode_Hub* HubNode = NodeCreator.CreateNode();
    HubNode->NodePosX = 0;
    HubNode->NodePosY = 0;
    HubNode->CreateRuntimeNode();
    NodeCreator.Finalize();
}

FLinearColor UNarrativeEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
    return FLinearColor(1.0f, 0.7f, 0.0f);
}

void UNarrativeEdGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
    const FScopedTransaction Transaction(LOCTEXT("BreakPinLinks", "Break Pin Links"));

    Super::BreakPinLinks(TargetPin, bSendsNodeNotification);

    if (UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(TargetPin.GetOwningNode()->GetGraph()))
    {
        NarrativeGraph->NotifyGraphChanged();
    }
}

void UNarrativeEdGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
    const FScopedTransaction Transaction(LOCTEXT("BreakSinglePinLink", "Break Pin Link"));

    Super::BreakSinglePinLink(SourcePin, TargetPin);

    if (SourcePin && SourcePin->GetOwningNode())
    {
        if (UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(SourcePin->GetOwningNode()->GetGraph()))
        {
            NarrativeGraph->NotifyGraphChanged();
        }
    }
}

class FConnectionDrawingPolicy* UNarrativeEdGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
    return nullptr;
}

void UNarrativeEdGraphSchema::DroppedAssetsOnGraph(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const
{
}

void UNarrativeEdGraphSchema::GetAssetsGraphHoverMessage(const TArray<FAssetData>& Assets, const UEdGraph* HoverGraph, FString& OutTooltipText, bool& OutOkIcon) const
{
    OutOkIcon = false;
}

UEdGraphNode* FNarrativeGraphSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(ParentGraph);
    if (!NarrativeGraph || !NodeClass)
    {
        return nullptr;
    }

    const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Narrative Node"));
    ParentGraph->Modify();

    UNarrativeEdGraphNode* GraphNode = nullptr;
    
    if (NodeClass == UNarrativeNode_Dialogue::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Dialogue>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Hub::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Hub>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Quest::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Quest>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Objective::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Objective>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Condition::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Condition>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Branch::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Branch>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Event::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Event>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Reward::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Reward>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_SetVariable::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_SetVariable>(ParentGraph);
    }
    else if (NodeClass == UNarrativeNode_Random::StaticClass())
    {
        GraphNode = NewObject<UNarrativeEdGraphNode_Random>(ParentGraph);
    }
    else
    {
        GraphNode = NewObject<UNarrativeEdGraphNode>(ParentGraph);
    }

    if (GraphNode)
    {
        GraphNode->CreateRuntimeNode();
        
        GraphNode->NodePosX = Location.X;
        GraphNode->NodePosY = Location.Y;
        GraphNode->CreateNewGuid();
        GraphNode->AllocateDefaultPins();

        ParentGraph->AddNode(GraphNode, true, bSelectNewNode);

        if (FromPin)
        {
            GraphNode->AutowireNewNode(FromPin);
        }

        NarrativeGraph->NotifyGraphChanged();

        return GraphNode;
    }

    return nullptr;
}

UEdGraphNode* FNarrativeGraphSchemaAction_NewComment::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    UEdGraphNode_Comment* CommentNode = NewObject<UEdGraphNode_Comment>(ParentGraph);
    
    if (CommentNode)
    {
        CommentNode->NodePosX = Location.X;
        CommentNode->NodePosY = Location.Y;
        CommentNode->NodeWidth = 400;
        CommentNode->NodeHeight = 200;
        CommentNode->NodeComment = TEXT("Comment");
        CommentNode->CommentColor = FLinearColor::White;

        ParentGraph->AddNode(CommentNode, true, bSelectNewNode);
    }

    return CommentNode;
}

#undef LOCTEXT_NAMESPACE
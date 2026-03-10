// Copyright Maxime Freteau. All Rights Reserved.

#include "Graph/NarrativeEdGraph.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "Core/NarrativeAsset.h"
#include "Core/NarrativeNode.h"
#include "EdGraph/EdGraphSchema.h"

void UNarrativeEdGraph::CompileGraph()
{
    if (!NarrativeAsset)
    {
        return;
    }

    NarrativeAsset->Nodes.Empty();
    NarrativeAsset->RootNode = nullptr;

    TMap<UEdGraphNode*, UNarrativeNode*> EditorToRuntimeMap;

    for (UEdGraphNode* GraphNode : Nodes)
    {
        if (UNarrativeEdGraphNode* NarrativeGraphNode = Cast<UNarrativeEdGraphNode>(GraphNode))
        {
            if (UNarrativeNode* RuntimeNode = NarrativeGraphNode->GetRuntimeNode())
            {
                NarrativeAsset->Nodes.Add(RuntimeNode);
                EditorToRuntimeMap.Add(GraphNode, RuntimeNode);

                if (NarrativeGraphNode->IsRootNode())
                {
                    NarrativeAsset->RootNode = RuntimeNode;
                }
            }
        }
    }

    for (UEdGraphNode* GraphNode : Nodes)
    {
        if (UNarrativeEdGraphNode* NarrativeGraphNode = Cast<UNarrativeEdGraphNode>(GraphNode))
        {
            UNarrativeNode* RuntimeNode = EditorToRuntimeMap.FindRef(GraphNode);
            if (!RuntimeNode)
            {
                continue;
            }

            RuntimeNode->OutputNodes.Empty();

            for (UEdGraphPin* Pin : GraphNode->Pins)
            {
                if (Pin->Direction == EGPD_Output)
                {
                    for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
                    {
                        if (LinkedPin && LinkedPin->GetOwningNode())
                        {
                            UNarrativeNode* LinkedRuntimeNode = EditorToRuntimeMap.FindRef(LinkedPin->GetOwningNode());
                            if (LinkedRuntimeNode)
                            {
                                RuntimeNode->AddOutputNode(LinkedRuntimeNode);
                            }
                        }
                    }
                }
            }
        }
    }

    NarrativeAsset->BuildMetadata();

    NarrativeAsset->MarkPackageDirty();
}

UNarrativeEdGraphNode* UNarrativeEdGraph::FindRootNode() const
{
    for (UEdGraphNode* Node : Nodes)
    {
        if (UNarrativeEdGraphNode* NarrativeNode = Cast<UNarrativeEdGraphNode>(Node))
        {
            if (NarrativeNode->IsRootNode())
            {
                return NarrativeNode;
            }
        }
    }
    return nullptr;
}

void UNarrativeEdGraph::NotifyGraphChanged()
{
    Super::NotifyGraphChanged();
}

#if WITH_EDITOR
void UNarrativeEdGraph::PostEditUndo()
{
    Super::PostEditUndo();
    NotifyGraphChanged();
}
#endif
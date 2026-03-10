// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Condition.h"
#include "Nodes/NarrativeNode_Condition.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Condition::UNarrativeEdGraphNode_Condition()
{
}

void UNarrativeEdGraphNode_Condition::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Condition>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}
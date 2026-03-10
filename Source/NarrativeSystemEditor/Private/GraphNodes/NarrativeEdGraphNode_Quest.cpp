// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Quest.h"
#include "Nodes/NarrativeNode_Quest.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Quest::UNarrativeEdGraphNode_Quest()
{
}

void UNarrativeEdGraphNode_Quest::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Quest>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}
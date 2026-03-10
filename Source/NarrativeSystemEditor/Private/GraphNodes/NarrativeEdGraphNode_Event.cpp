// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Event.h"
#include "Nodes/NarrativeNode_Event.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Event::UNarrativeEdGraphNode_Event()
{
}

void UNarrativeEdGraphNode_Event::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Event>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}
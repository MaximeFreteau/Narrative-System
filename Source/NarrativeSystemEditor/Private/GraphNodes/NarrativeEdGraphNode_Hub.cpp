// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Hub.h"
#include "Nodes/NarrativeNode_Hub.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Hub::UNarrativeEdGraphNode_Hub()
{
}

void UNarrativeEdGraphNode_Hub::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Hub>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}
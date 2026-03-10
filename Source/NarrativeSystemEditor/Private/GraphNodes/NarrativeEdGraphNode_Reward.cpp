// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Reward.h"
#include "Nodes/NarrativeNode_Reward.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Reward::UNarrativeEdGraphNode_Reward()
{
}

void UNarrativeEdGraphNode_Reward::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Reward>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}
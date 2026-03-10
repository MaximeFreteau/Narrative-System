// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Objective.h"
#include "Nodes/NarrativeNode_Objective.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Objective::UNarrativeEdGraphNode_Objective()
{
}

void UNarrativeEdGraphNode_Objective::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Objective>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}
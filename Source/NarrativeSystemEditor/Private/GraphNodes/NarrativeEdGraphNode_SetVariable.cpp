// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_SetVariable.h"
#include "Nodes/NarrativeNode_SetVariable.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_SetVariable::UNarrativeEdGraphNode_SetVariable()
{
}

void UNarrativeEdGraphNode_SetVariable::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_SetVariable>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}
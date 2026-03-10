// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Random.h"
#include "Nodes/NarrativeNode_Random.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Random::UNarrativeEdGraphNode_Random()
{
}

void UNarrativeEdGraphNode_Random::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Random>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}

int32 UNarrativeEdGraphNode_Random::GetOutputPinCount() const
{
	if (!RuntimeNode)
	{
		return 2;
	}

	if (UNarrativeNode_Random* RandomNode = Cast<UNarrativeNode_Random>(RuntimeNode))
	{
		return FMath::Max(2, RandomNode->OutputWeights.Num());
	}
	return 2;
}
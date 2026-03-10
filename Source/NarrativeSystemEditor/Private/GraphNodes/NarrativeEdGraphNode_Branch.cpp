// Copyright Maxime Freteau. All Rights Reserved.

#include "GraphNodes/NarrativeEdGraphNode_Branch.h"
#include "Nodes/NarrativeNode_Branch.h"
#include "Graph/NarrativeEdGraph.h"
#include "Core/NarrativeAsset.h"

UNarrativeEdGraphNode_Branch::UNarrativeEdGraphNode_Branch()
{
}

void UNarrativeEdGraphNode_Branch::CreateRuntimeNode()
{
	UNarrativeEdGraph* NarrativeGraph = Cast<UNarrativeEdGraph>(GetGraph());
	if (!NarrativeGraph || !NarrativeGraph->NarrativeAsset)
	{
		return;
	}

	RuntimeNode = NewObject<UNarrativeNode_Branch>(NarrativeGraph->NarrativeAsset);
	RuntimeNode->GraphNode = this;
}

void UNarrativeEdGraphNode_Branch::AllocateDefaultPins()
{
	if (!RuntimeNode)
	{
		CreateRuntimeNode();
	}

	FCreatePinParams InputParams;
	InputParams.Index = 0;
	UEdGraphNode::CreatePin(EGPD_Input, TEXT("Narrative"), TEXT("In"), InputParams);

	FCreatePinParams TrueParams;
	TrueParams.Index = 0;
	UEdGraphPin* TruePin = UEdGraphNode::CreatePin(EGPD_Output, TEXT("Narrative"), TEXT("True"), TrueParams);
	TruePin->PinFriendlyName = FText::FromString("True");

	FCreatePinParams FalseParams;
	FalseParams.Index = 1;
	UEdGraphPin* FalsePin = UEdGraphNode::CreatePin(EGPD_Output, TEXT("Narrative"), TEXT("False"), FalseParams);
	FalsePin->PinFriendlyName = FText::FromString("False");
}
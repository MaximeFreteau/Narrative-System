// Copyright Maxime Freteau. All Rights Reserved.

#include "Core/NarrativeNode.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode::UNarrativeNode()
{
	NodeTitle = FText::FromString("Narrative Node");
	NodeGuid = FGuid::NewGuid();
}

UNarrativeNode* UNarrativeNode::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
	return GetDefaultNextNode();
}

bool UNarrativeNode::CanExecute_Implementation(UNarrativeComponent* NarrativeComp) const
{
	return true;
}

void UNarrativeNode::OnNodeCompleted_Implementation(UNarrativeComponent* NarrativeComp)
{
}

UNarrativeNode* UNarrativeNode::GetOutputNode(int32 Index) const
{
	if (OutputNodes.IsValidIndex(Index))
	{
		return OutputNodes[Index];
	}
	return nullptr;
}

void UNarrativeNode::AddOutputNode(UNarrativeNode* Node)
{
	if (Node && !OutputNodes.Contains(Node))
	{
		OutputNodes.Add(Node);
		Node->InputNodes.AddUnique(this);
	}
}

void UNarrativeNode::RemoveOutputNode(UNarrativeNode* Node)
{
	if (Node)
	{
		OutputNodes.Remove(Node);
		Node->InputNodes.Remove(this);
	}
}

UNarrativeNode* UNarrativeNode::GetDefaultNextNode() const
{
	return GetOutputNode(0);
}
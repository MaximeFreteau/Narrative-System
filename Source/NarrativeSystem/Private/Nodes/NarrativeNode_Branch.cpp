// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Branch.h"
#include "Nodes/NarrativeNode_Condition.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Branch::UNarrativeNode_Branch()
{
	NodeTitle = FText::FromString("Branch");
    
	Condition = CreateDefaultSubobject<UNarrativeNode_Condition>(TEXT("DefaultCondition"));
}

UNarrativeNode* UNarrativeNode_Branch::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
	if (!NarrativeComp || !Condition)
	{
		return nullptr;
	}

	bool bConditionResult = Condition->EvaluateCondition(NarrativeComp);

	int32 OutputIndex = bConditionResult ? 0 : 1;
    
	return GetOutputNode(OutputIndex);
}

#if WITH_EDITOR
FText UNarrativeNode_Branch::GetNodeTitle() const
{
	return FText::FromString("Branch");
}

FLinearColor UNarrativeNode_Branch::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.7f, 0.2f);
}

FText UNarrativeNode_Branch::GetNodeTooltip() const
{
	return FText::FromString("Branch Node\n\nEvaluates a condition and takes different paths.\nOutput 0 = True\nOutput 1 = False");
}
#endif
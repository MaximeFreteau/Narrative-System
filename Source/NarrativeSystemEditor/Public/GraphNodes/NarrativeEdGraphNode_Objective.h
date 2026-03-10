// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "NarrativeEdGraphNode_Objective.generated.h"

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraphNode_Objective : public UNarrativeEdGraphNode
{
	GENERATED_BODY()

public:
	UNarrativeEdGraphNode_Objective();

	virtual void CreateRuntimeNode() override;
};
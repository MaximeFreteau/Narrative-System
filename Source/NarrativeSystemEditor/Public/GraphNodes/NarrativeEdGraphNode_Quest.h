// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "NarrativeEdGraphNode_Quest.generated.h"

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraphNode_Quest : public UNarrativeEdGraphNode
{
	GENERATED_BODY()

public:
	UNarrativeEdGraphNode_Quest();

	virtual void CreateRuntimeNode() override;
};
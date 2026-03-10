// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "NarrativeEdGraphNode_SetVariable.generated.h"

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraphNode_SetVariable : public UNarrativeEdGraphNode
{
	GENERATED_BODY()

public:
	UNarrativeEdGraphNode_SetVariable();

	virtual void CreateRuntimeNode() override;
};
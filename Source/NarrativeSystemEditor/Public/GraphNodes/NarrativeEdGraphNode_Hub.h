// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "NarrativeEdGraphNode_Hub.generated.h"

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraphNode_Hub : public UNarrativeEdGraphNode
{
	GENERATED_BODY()

public:
	UNarrativeEdGraphNode_Hub();

	virtual void CreateRuntimeNode() override;
	virtual bool IsRootNode() const override { return true; }
	virtual int32 GetInputPinCount() const override { return 0; }
};
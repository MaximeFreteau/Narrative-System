// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "NarrativeEdGraphNode_Dialogue.generated.h"

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraphNode_Dialogue : public UNarrativeEdGraphNode
{
	GENERATED_BODY()

public:
	UNarrativeEdGraphNode_Dialogue();

	virtual void CreateRuntimeNode() override;
	virtual int32 GetOutputPinCount() const override;
	virtual void AllocateDefaultPins() override;

	UFUNCTION()
	void RefreshPins();
};
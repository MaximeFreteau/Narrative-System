// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "NarrativeEdGraph.generated.h"

class UNarrativeAsset;

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UNarrativeAsset> NarrativeAsset;

	void CompileGraph();

	class UNarrativeEdGraphNode* FindRootNode() const;

	void NotifyGraphChanged();

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif
};
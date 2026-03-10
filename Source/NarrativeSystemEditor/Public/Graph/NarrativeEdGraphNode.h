// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "Editor.h"
#include "NarrativeEdGraphNode.generated.h"

class UNarrativeNode;

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UNarrativeEdGraphNode();
	virtual void CreateRuntimeNode();

	UPROPERTY()
	TObjectPtr<UNarrativeNode> RuntimeNode;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual void PrepareForCopying() override;
	virtual void PostPasteNode() override;
	virtual void DestroyNode() override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;  // AJOUTER CETTE LIGNE

	virtual bool IsRootNode() const { return false; }

	UNarrativeNode* GetRuntimeNode() const { return RuntimeNode; }

	virtual int32 GetInputPinCount() const { return 1; }

	virtual int32 GetOutputPinCount() const { return 1; }

	virtual void ReconstructNode() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	void CreateInputPins();
	void CreateOutputPins();

};
// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "NarrativeEdGraphSchema.generated.h"

UCLASS()
class NARRATIVESYSTEMEDITOR_API UNarrativeEdGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

public:
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
    virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
    virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
    virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
    virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
    virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
    virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
    virtual void DroppedAssetsOnGraph(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const override;
    virtual void GetAssetsGraphHoverMessage(const TArray<FAssetData>& Assets, const UEdGraph* HoverGraph, FString& OutTooltipText, bool& OutOkIcon) const override;

private:
    void GetNarrativeNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder) const;
    void GetCommentAction(FGraphContextMenuBuilder& ContextMenuBuilder) const;
};

USTRUCT()
struct NARRATIVESYSTEMEDITOR_API FNarrativeGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    UPROPERTY()
    TSubclassOf<class UNarrativeNode> NodeClass;

    FNarrativeGraphSchemaAction_NewNode()
        : FEdGraphSchemaAction()
        , NodeClass(nullptr)
    {}

    FNarrativeGraphSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
        : FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
        , NodeClass(nullptr)
    {}

    virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
};

USTRUCT()
struct NARRATIVESYSTEMEDITOR_API FNarrativeGraphSchemaAction_NewComment : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FNarrativeGraphSchemaAction_NewComment()
        : FEdGraphSchemaAction()
    {}

    FNarrativeGraphSchemaAction_NewComment(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
        : FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
    {}

    virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
};
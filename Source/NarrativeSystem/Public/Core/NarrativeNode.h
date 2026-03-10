// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NarrativeTypes.h"
#include "NarrativeNode.generated.h"

class UNarrativeComponent;
class UEdGraphNode;

UCLASS(Abstract, BlueprintType, Blueprintable)
class NARRATIVESYSTEM_API UNarrativeNode : public UObject
{
    GENERATED_BODY()

public:
    UNarrativeNode();

    UPROPERTY(EditAnywhere, Category = "Node")
    FText NodeTitle;

    UPROPERTY(EditAnywhere, Category = "Node", meta = (MultiLine = true))
    FText NodeDescription;

    UPROPERTY()
    FGuid NodeGuid;

    UPROPERTY()
    TArray<TObjectPtr<UNarrativeNode>> OutputNodes;

    UPROPERTY()
    TArray<TObjectPtr<UNarrativeNode>> InputNodes;

    UPROPERTY()
    TObjectPtr<UEdGraphNode> GraphNode;

    UFUNCTION(BlueprintNativeEvent, Category = "Narrative")
    UNarrativeNode* Execute(UNarrativeComponent* NarrativeComp);
    virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp);

    UFUNCTION(BlueprintNativeEvent, Category = "Narrative")
    bool CanExecute(UNarrativeComponent* NarrativeComp) const;
    virtual bool CanExecute_Implementation(UNarrativeComponent* NarrativeComp) const;

    UFUNCTION(BlueprintNativeEvent, Category = "Narrative")
    void OnNodeCompleted(UNarrativeComponent* NarrativeComp);
    virtual void OnNodeCompleted_Implementation(UNarrativeComponent* NarrativeComp);

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    TArray<UNarrativeNode*> GetOutputNodes() const { return OutputNodes; }

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    UNarrativeNode* GetOutputNode(int32 Index) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    int32 GetNumOutputs() const { return OutputNodes.Num(); }

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void AddOutputNode(UNarrativeNode* Node);

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void RemoveOutputNode(UNarrativeNode* Node);

#if WITH_EDITOR
    virtual FText GetNodeTitle() const { return NodeTitle; }
    virtual FLinearColor GetNodeTitleColor() const { return FLinearColor::White; }
    virtual FText GetNodeTooltip() const { return NodeDescription; }
    virtual bool CanUserDeleteNode() const { return true; }
#endif

protected:
    UNarrativeNode* GetDefaultNextNode() const;
};
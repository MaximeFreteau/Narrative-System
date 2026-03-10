// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeNode_Condition.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Condition : public UNarrativeNode
{
    GENERATED_BODY()

public:
    UNarrativeNode_Condition();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    EConditionType ConditionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    FGameplayTagQuery TagQuery;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    int32 CompareValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    EComparisonOp ComparisonOp = EComparisonOp::Equal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    FName VariableName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    TArray<TObjectPtr<UNarrativeNode_Condition>> SubConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    ELogicalOp LogicalOp = ELogicalOp::And;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (EditCondition = "ConditionType == EConditionType::HasTag || ConditionType == EConditionType::HasAllTags || ConditionType == EConditionType::HasAnyTag"))
    FGameplayTagContainer TagsToCheck;

    virtual bool CanExecute_Implementation(UNarrativeComponent* NarrativeComp) const override;

    UFUNCTION(BlueprintCallable, Category = "Condition")
    bool EvaluateCondition(UNarrativeComponent* NarrativeComp) const;

#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetNodeTooltip() const override;
#endif

private:
    bool EvaluateSimpleCondition(UNarrativeComponent* NarrativeComp) const;
    bool CompareInt(int32 A, int32 B, EComparisonOp Op) const;
};
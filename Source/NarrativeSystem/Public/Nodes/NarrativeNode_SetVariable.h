// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeNode_SetVariable.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_SetVariable : public UNarrativeNode
{
    GENERATED_BODY()

public:
    UNarrativeNode_SetVariable();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
    FName VariableName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
    EVariableType VariableType = EVariableType::Int;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
    ESetOperation Operation = ESetOperation::Set;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (EditCondition = "VariableType == EVariableType::Int", EditConditionHides))
    int32 IntValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (EditCondition = "VariableType == EVariableType::Float", EditConditionHides))
    float FloatValue = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (EditCondition = "VariableType == EVariableType::Bool", EditConditionHides))
    bool BoolValue = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (EditCondition = "VariableType == EVariableType::String", EditConditionHides))
    FString StringValue;

    virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetNodeTooltip() const override;
#endif

private:
    void SetIntVariable(UNarrativeComponent* NarrativeComp);
    void SetFloatVariable(UNarrativeComponent* NarrativeComp);
    void SetBoolVariable(UNarrativeComponent* NarrativeComp);
    void SetStringVariable(UNarrativeComponent* NarrativeComp);
};
// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_SetVariable.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_SetVariable::UNarrativeNode_SetVariable()
{
    NodeTitle = FText::FromString("Set Variable");
    VariableName = FName("MyVariable");
    VariableType = EVariableType::Int;
    Operation = ESetOperation::Set;
    IntValue = 0;
    FloatValue = 0.f;
    BoolValue = false;
}

UNarrativeNode* UNarrativeNode_SetVariable::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return nullptr;
    }

    switch (VariableType)
    {
        case EVariableType::Int:
            SetIntVariable(NarrativeComp);
            break;
        case EVariableType::Float:
            SetFloatVariable(NarrativeComp);
            break;
        case EVariableType::Bool:
            SetBoolVariable(NarrativeComp);
            break;
        case EVariableType::String:
            SetStringVariable(NarrativeComp);
            break;
    }

    return GetDefaultNextNode();
}

void UNarrativeNode_SetVariable::SetIntVariable(UNarrativeComponent* NarrativeComp)
{
    int32 CurrentValue = NarrativeComp->GetIntVariable(VariableName);
    int32 NewValue = IntValue;

    switch (Operation)
    {
        case ESetOperation::Set:
            NewValue = IntValue;
            break;
        case ESetOperation::Add:
            NewValue = CurrentValue + IntValue;
            break;
        case ESetOperation::Subtract:
            NewValue = CurrentValue - IntValue;
            break;
        case ESetOperation::Multiply:
            NewValue = CurrentValue * IntValue;
            break;
        case ESetOperation::Divide:
            NewValue = (IntValue != 0) ? (CurrentValue / IntValue) : CurrentValue;
            break;
    }

    NarrativeComp->SetIntVariable(VariableName, NewValue);
}

void UNarrativeNode_SetVariable::SetFloatVariable(UNarrativeComponent* NarrativeComp)
{
    float CurrentValue = NarrativeComp->GetFloatVariable(VariableName);
    float NewValue = FloatValue;

    switch (Operation)
    {
        case ESetOperation::Set:
            NewValue = FloatValue;
            break;
        case ESetOperation::Add:
            NewValue = CurrentValue + FloatValue;
            break;
        case ESetOperation::Subtract:
            NewValue = CurrentValue - FloatValue;
            break;
        case ESetOperation::Multiply:
            NewValue = CurrentValue * FloatValue;
            break;
        case ESetOperation::Divide:
            NewValue = (FloatValue != 0.f) ? (CurrentValue / FloatValue) : CurrentValue;
            break;
    }

    NarrativeComp->SetFloatVariable(VariableName, NewValue);
}

void UNarrativeNode_SetVariable::SetBoolVariable(UNarrativeComponent* NarrativeComp)
{
    NarrativeComp->SetBoolVariable(VariableName, BoolValue);
}

void UNarrativeNode_SetVariable::SetStringVariable(UNarrativeComponent* NarrativeComp)
{
    NarrativeComp->SetStringVariable(VariableName, StringValue);
}

#if WITH_EDITOR
FText UNarrativeNode_SetVariable::GetNodeTitle() const
{
    FString OpStr;
    switch (Operation)
    {
        case ESetOperation::Set: OpStr = TEXT("="); break;
        case ESetOperation::Add: OpStr = TEXT("+="); break;
        case ESetOperation::Subtract: OpStr = TEXT("-="); break;
        case ESetOperation::Multiply: OpStr = TEXT("*="); break;
        case ESetOperation::Divide: OpStr = TEXT("/="); break;
    }

    return FText::Format(
        FText::FromString("Set: {0} {1}"),
        FText::FromName(VariableName),
        FText::FromString(OpStr)
    );
}

FLinearColor UNarrativeNode_SetVariable::GetNodeTitleColor() const
{
    return FLinearColor(0.3f, 0.8f, 0.8f);
}

FText UNarrativeNode_SetVariable::GetNodeTooltip() const
{
    FString TypeStr;
    FString ValueStr;

    switch (VariableType)
    {
        case EVariableType::Int:
            TypeStr = TEXT("Int");
            ValueStr = FString::Printf(TEXT("%d"), IntValue);
            break;
        case EVariableType::Float:
            TypeStr = TEXT("Float");
            ValueStr = FString::Printf(TEXT("%.2f"), FloatValue);
            break;
        case EVariableType::Bool:
            TypeStr = TEXT("Bool");
            ValueStr = BoolValue ? TEXT("true") : TEXT("false");
            break;
        case EVariableType::String:
            TypeStr = TEXT("String");
            ValueStr = StringValue;
            break;
    }

    FString OpStr;
    switch (Operation)
    {
        case ESetOperation::Set: OpStr = TEXT("Set to"); break;
        case ESetOperation::Add: OpStr = TEXT("Add"); break;
        case ESetOperation::Subtract: OpStr = TEXT("Subtract"); break;
        case ESetOperation::Multiply: OpStr = TEXT("Multiply by"); break;
        case ESetOperation::Divide: OpStr = TEXT("Divide by"); break;
    }

    return FText::Format(
        FText::FromString("Set Variable ({0})\n{1} {2} {3}"),
        FText::FromString(TypeStr),
        FText::FromName(VariableName),
        FText::FromString(OpStr),
        FText::FromString(ValueStr)
    );
}
#endif
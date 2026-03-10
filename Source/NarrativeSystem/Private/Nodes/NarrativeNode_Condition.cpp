// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Condition.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Condition::UNarrativeNode_Condition()
{
    NodeTitle = FText::FromString("Condition");
    ConditionType = EConditionType::VariableCheck;
    CompareValue = 0;
    ComparisonOp = EComparisonOp::Equal;
}

bool UNarrativeNode_Condition::CanExecute_Implementation(UNarrativeComponent* NarrativeComp) const
{
    return EvaluateCondition(NarrativeComp);
}

bool UNarrativeNode_Condition::EvaluateCondition(UNarrativeComponent* Component) const
{
    if (!Component)
    {
        return false;
    }

    switch (ConditionType)
    {
        case EConditionType::HasTag:
        {
            if (TagsToCheck.Num() > 0)
            {
                return Component->HasGameplayTag(TagsToCheck.First());
            }
            return false;
        }

        case EConditionType::HasAllTags:
        {
            return Component->HasAllGameplayTags(TagsToCheck);
        }

        case EConditionType::HasAnyTag:
        {
            return Component->HasAnyGameplayTag(TagsToCheck);
        }

        case EConditionType::Simple:
        {
            return Component->GetBoolVariable(VariableName);
        }

        case EConditionType::Complex:
        {
            if (SubConditions.Num() == 0)
            {
                return true;
            }

            bool Result = true;
            bool bFirst = true;

            for (const TObjectPtr<UNarrativeNode_Condition>& SubNode : SubConditions)
            {
                if (!SubNode)
                    continue;

                bool SubResult = SubNode->EvaluateCondition(Component);

                if (bFirst)
                {
                    Result = SubResult;
                    bFirst = false;
                }
                else
                {
                    Result = Result && SubResult;
                }
            }

            return Result;
        }

        case EConditionType::CompareInt:
        {
            int32 VarValue = Component->GetIntVariable(VariableName);
            return CompareInt(VarValue, CompareValue, ComparisonOp);
        }

        case EConditionType::CompareBool:
        {
            bool VarValue = Component->GetBoolVariable(VariableName);
            return (VarValue == (CompareValue > 0));
        }

        case EConditionType::CompareFloat:
        {
            float VarValue = Component->GetFloatVariable(VariableName);
            return CompareInt(FMath::RoundToInt(VarValue), CompareValue, ComparisonOp);
        }

        case EConditionType::CompareString:
        {
            FString VarValue = Component->GetStringVariable(VariableName);
            FString CompareStr = FString::FromInt(CompareValue);

            switch (ComparisonOp)
            {
                case EComparisonOp::Equal:
                    return VarValue.Equals(CompareStr);
                case EComparisonOp::NotEqual:
                    return !VarValue.Equals(CompareStr);
                default:
                    return false;
            }
        }

        case EConditionType::CheckQuestState:
        {
            EQuestState CurrentState = Component->GetQuestState(VariableName);
            EQuestState ExpectedState = static_cast<EQuestState>(CompareValue);
            return CurrentState == ExpectedState;
        }

        case EConditionType::Custom:
        default:
            return false;
    }
}


bool UNarrativeNode_Condition::CompareInt(int32 A, int32 B, EComparisonOp Op) const
{
    switch (Op)
    {
        case EComparisonOp::Equal:
            return A == B;
        case EComparisonOp::NotEqual:
            return A != B;
        case EComparisonOp::Greater:
            return A > B;
        case EComparisonOp::GreaterOrEqual:
            return A >= B;
        case EComparisonOp::Less:
            return A < B;
        case EComparisonOp::LessOrEqual:
            return A <= B;
        default:
            return false;
    }
}

bool UNarrativeNode_Condition::EvaluateSimpleCondition(UNarrativeComponent* NarrativeComp) const
{
    switch (ConditionType)
    {
        case EConditionType::VariableCheck:
        {
            int32 VarValue = NarrativeComp->GetIntVariable(VariableName);
            return CompareInt(VarValue, CompareValue, ComparisonOp);
        }

        case EConditionType::CheckQuestState:
        {
            EQuestState State = NarrativeComp->GetQuestState(VariableName);
            return CompareInt(static_cast<int32>(State), CompareValue, ComparisonOp);
        }

        case EConditionType::ReputationCheck:
        {
            int32 Reputation = NarrativeComp->GetNPCReputation(VariableName);
            return CompareInt(Reputation, CompareValue, ComparisonOp);
        }

        default:
            return true;
    }
}

#if WITH_EDITOR
FText UNarrativeNode_Condition::GetNodeTitle() const
{
    FString TypeStr;
    switch (ConditionType)
    {
        case EConditionType::PlayerLevel: TypeStr = TEXT("Level"); break;
        case EConditionType::VariableCheck: TypeStr = TEXT("Variable"); break;
        case EConditionType::CheckQuestState: TypeStr = TEXT("Quest State"); break;
        default: TypeStr = TEXT("Condition"); break;
    }

    return FText::Format(FText::FromString("Condition: {0}"), FText::FromString(TypeStr));
}

FLinearColor UNarrativeNode_Condition::GetNodeTitleColor() const
{
    return FLinearColor(1.0f, 0.5f, 0.0f);
}

FText UNarrativeNode_Condition::GetNodeTooltip() const
{
    FString TypeStr;
    
    switch (ConditionType)
    {
    case EConditionType::HasTag: TypeStr = TEXT("Has Tag"); break;
    case EConditionType::HasAllTags: TypeStr = TEXT("Has All Tags"); break;
    case EConditionType::HasAnyTag: TypeStr = TEXT("Has Any Tag"); break;
    case EConditionType::Simple: TypeStr = TEXT("Simple"); break;
    case EConditionType::Complex: TypeStr = TEXT("Complex"); break;
    case EConditionType::CompareInt: TypeStr = TEXT("Int"); break;
    case EConditionType::CompareBool: TypeStr = TEXT("Bool"); break;
    case EConditionType::CompareFloat: TypeStr = TEXT("Float"); break;
    case EConditionType::CompareString: TypeStr = TEXT("String"); break;
    case EConditionType::CheckQuestState: TypeStr = TEXT("Quest State"); break;
    case EConditionType::Custom: TypeStr = TEXT("Custom"); break;
    default: TypeStr = TEXT("Unknown"); break;
    }

    return FText::FromString(FString::Printf(TEXT("Condition: %s"), *TypeStr));
}
#endif
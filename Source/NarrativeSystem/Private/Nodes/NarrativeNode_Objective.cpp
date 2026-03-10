// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Objective.h"
#include "Core/NarrativeComponent.h"
#include "GameplayTagsManager.h"

UNarrativeNode_Objective::UNarrativeNode_Objective()
{
    NodeTitle = FText::FromString("Objective");
    
    ObjectiveID = FName(*FString::Printf(TEXT("Objective_%d"), FMath::Rand()));
    
    ObjectiveInfo.ObjectiveDescription = FText::FromString("New Objective");
    ObjectiveInfo.ObjectiveType = EObjectiveType::Custom;
    RequiredCount = 1;
    bOptional = false;
    bHidden = false;
    bAutoCompleteOnStart = false;
    
    CurrentProgress = 0;
    bIsCompleted = false;
}

UNarrativeNode* UNarrativeNode_Objective::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return nullptr;
    }

    NarrativeComp->OnObjectiveStarted.Broadcast(ObjectiveInfo.ObjectiveDescription, bOptional);

    if (bAutoCompleteOnStart)
    {
        ForceComplete(NarrativeComp);
        return GetOutputNode(0);
    }

    if (bOptional || RequiredCount == 0)
    {
        return GetOutputNode(0);
    }
    return nullptr;
}

void UNarrativeNode_Objective::UpdateProgress(UNarrativeComponent* NarrativeComp, int32 Amount)
{
    if (!NarrativeComp || bIsCompleted)
    {
        return;
    }

    CurrentProgress = FMath::Clamp(CurrentProgress + Amount, 0, RequiredCount);

    NarrativeComp->OnObjectiveUpdated.Broadcast(TargetTag, CurrentProgress, RequiredCount);

    if (CurrentProgress >= RequiredCount)
    {
        bIsCompleted = true;
        
        NarrativeComp->OnObjectiveCompleted.Broadcast(ObjectiveInfo.ObjectiveDescription);

        if (UNarrativeNode* NextNode = GetOutputNode(0))
        {
            NextNode->Execute(NarrativeComp);
        }
    }
}

void UNarrativeNode_Objective::ForceComplete(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp || bIsCompleted)
    {
        return;
    }

    CurrentProgress = RequiredCount;
    bIsCompleted = true;
    
    NarrativeComp->OnObjectiveUpdated.Broadcast(TargetTag, CurrentProgress, RequiredCount);
    NarrativeComp->OnObjectiveCompleted.Broadcast(ObjectiveInfo.ObjectiveDescription);
}

void UNarrativeNode_Objective::ResetProgress()
{
    CurrentProgress = 0;
    bIsCompleted = false;
}

float UNarrativeNode_Objective::GetCompletionPercentage() const
{
    if (RequiredCount <= 0)
    {
        return 1.0f;
    }
    return FMath::Clamp((float)CurrentProgress / (float)RequiredCount, 0.0f, 1.0f);
}

bool UNarrativeNode_Objective::MatchesObjectiveCriteria(AActor* Actor, FName ItemID, FName LocationID) const
{
    switch (ObjectiveInfo.ObjectiveType)
    {
        case EObjectiveType::DefeatEnemy:
        {
            if (!Actor)
                return false;
            
            if (ObjectiveInfo.EnemyType != NAME_None)
            {
                FString ActorName = Actor->GetName();
                if (!ActorName.Contains(ObjectiveInfo.EnemyType.ToString()))
                {
                    return false;
                }
            }

            return true;
        }

        case EObjectiveType::CollectItem:
        {
            return ItemID == ObjectiveInfo.ItemID;
        }

        case EObjectiveType::TalkToNPC:
        {
            if (!Actor)
                return false;
            
            FString ActorName = Actor->GetName();
            return ActorName.Contains(ObjectiveInfo.NPCID.ToString());
        }

        case EObjectiveType::VisitLocation:
        {
            return LocationID == ObjectiveInfo.LocationID;
        }

        case EObjectiveType::Interact:
        {
            if (!Actor)
                return false;
            
            FString ActorName = Actor->GetName();
            return ActorName.Contains(ObjectiveInfo.InteractObjectID.ToString());
        }

        case EObjectiveType::Deliver:
        {
            return ItemID == ObjectiveInfo.DeliverItemID;
        }

        default:
            return false;
    }
}

#if WITH_EDITOR

FText UNarrativeNode_Objective::GetNodeTitle() const
{
    if (!ObjectiveInfo.ObjectiveDescription.IsEmpty())
    {
        return FText::Format(FText::FromString("Objective: {0}"), ObjectiveInfo.ObjectiveDescription);
    }
    return FText::FromString("Objective");
}

FLinearColor UNarrativeNode_Objective::GetNodeTitleColor() const
{
    switch (ObjectiveInfo.ObjectiveType)
    {
        case EObjectiveType::DefeatEnemy:
            return FLinearColor(1.0f, 0.3f, 0.3f);
        case EObjectiveType::CollectItem:
            return FLinearColor(1.0f, 0.8f, 0.0f);
        case EObjectiveType::TalkToNPC:
            return FLinearColor(0.3f, 0.8f, 1.0f);
        case EObjectiveType::VisitLocation:
            return FLinearColor(0.5f, 1.0f, 0.5f);
        case EObjectiveType::Escort:
            return FLinearColor(1.0f, 0.5f, 0.8f);
        case EObjectiveType::Defend:
            return FLinearColor(0.8f, 0.3f, 0.0f);
        case EObjectiveType::Survive:
            return FLinearColor(0.9f, 0.0f, 0.0f);
        default:
            return FLinearColor(0.7f, 0.7f, 0.7f);
    }
}

FText UNarrativeNode_Objective::GetNodeTooltip() const
{
    FString Tooltip = FString::Printf(TEXT("Objective: %s\n"), *ObjectiveInfo.ObjectiveDescription.ToString());
    Tooltip += FString::Printf(TEXT("Type: %s\n"), *UEnum::GetValueAsString(ObjectiveInfo.ObjectiveType));
    
    if (bOptional)
    {
        Tooltip += TEXT("(Optional) ");
    }
    if (bHidden)
    {
        Tooltip += TEXT("(Hidden) ");
    }

    switch (ObjectiveInfo.ObjectiveType)
    {
        case EObjectiveType::DefeatEnemy:
            if (ObjectiveInfo.EnemyType != NAME_None)
            {
                Tooltip += FString::Printf(TEXT("\nEnemy: %s x%d"), *ObjectiveInfo.EnemyType.ToString(), ObjectiveInfo.EnemyCount);
            }
            break;
        case EObjectiveType::CollectItem:
            if (ObjectiveInfo.ItemID != NAME_None)
            {
                Tooltip += FString::Printf(TEXT("\nItem: %s x%d"), *ObjectiveInfo.ItemID.ToString(), ObjectiveInfo.ItemCount);
            }
            break;
        case EObjectiveType::TalkToNPC:
            if (!ObjectiveInfo.NPCName.IsEmpty())
            {
                Tooltip += FString::Printf(TEXT("\nNPC: %s"), *ObjectiveInfo.NPCName.ToString());
            }
            break;
        case EObjectiveType::VisitLocation:
            if (!ObjectiveInfo.LocationName.IsEmpty())
            {
                Tooltip += FString::Printf(TEXT("\nLocation: %s"), *ObjectiveInfo.LocationName.ToString());
            }
            break;
        default:
            break;
    }
    
    return FText::FromString(Tooltip);
}

void UNarrativeNode_Objective::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    if (ObjectiveID == NAME_None)
    {
        ObjectiveID = FName(*FString::Printf(TEXT("Objective_%d"), FMath::Rand()));
    }

    switch (ObjectiveInfo.ObjectiveType)
    {
        case EObjectiveType::DefeatEnemy:
            RequiredCount = ObjectiveInfo.EnemyCount;
            break;
        case EObjectiveType::CollectItem:
            RequiredCount = ObjectiveInfo.ItemCount;
            break;
        case EObjectiveType::Craft:
            RequiredCount = ObjectiveInfo.CraftCount;
            break;
        case EObjectiveType::Interact:
            RequiredCount = ObjectiveInfo.InteractCount;
            break;
        default:
            if (RequiredCount == 0)
            {
                RequiredCount = 1;
            }
            break;
    }
}

#endif

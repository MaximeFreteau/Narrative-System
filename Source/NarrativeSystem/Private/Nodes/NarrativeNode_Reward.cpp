// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Reward.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Reward::UNarrativeNode_Reward()
{
    NodeTitle = FText::FromString("Reward");
    bShowNotification = true;
}

UNarrativeNode* UNarrativeNode_Reward::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return nullptr;
    }

    NarrativeComp->OnRewardReceived.Broadcast(
        Rewards.ExperiencePoints,
        Rewards.Gold,
        Rewards.Items
    );

    if (bShowNotification)
    {
        FString RewardText;
        
        if (Rewards.ExperiencePoints > 0)
        {
            RewardText += FString::Printf(TEXT("%d XP"), Rewards.ExperiencePoints);
        }
        
        if (Rewards.Gold > 0)
        {
            if (!RewardText.IsEmpty()) RewardText += TEXT(", ");
            RewardText += FString::Printf(TEXT("%d Gold"), Rewards.Gold);
        }
        
        if (Rewards.Items.Num() > 0)
        {
            if (!RewardText.IsEmpty()) RewardText += TEXT(", ");
            RewardText += FString::Printf(TEXT("%d Items"), Rewards.Items.Num());
        }

        if (!RewardText.IsEmpty())
        {
            NarrativeComp->OnNotification.Broadcast(
                FText::FromString("Reward Received"),
                FText::FromString(RewardText),
                ENotificationType::Success,
                3.0f
            );
        }
    }

    return GetDefaultNextNode();
}



#if WITH_EDITOR
FText UNarrativeNode_Reward::GetNodeTitle() const
{
    return FText::FromString("Reward");
}

FLinearColor UNarrativeNode_Reward::GetNodeTitleColor() const
{
    return FLinearColor(1.0f, 0.84f, 0.0f);
}

FText UNarrativeNode_Reward::GetNodeTooltip() const
{
    FString TooltipText = TEXT("Reward Node\n\n");
    
    if (Rewards.ExperiencePoints > 0)
    {
        TooltipText += FString::Printf(TEXT("XP: %d\n"), Rewards.ExperiencePoints);
    }
    
    if (Rewards.Gold > 0)
    {
        TooltipText += FString::Printf(TEXT("Gold: %d\n"), Rewards.Gold);
    }
    
    if (Rewards.Items.Num() > 0)
    {
        TooltipText += FString::Printf(TEXT("Items: %d\n"), Rewards.Items.Num());
    }
    
    if (Rewards.UnlockTags.Num() > 0)
    {
        TooltipText += FString::Printf(TEXT("Unlocks: %d tags"), Rewards.UnlockTags.Num());
    }

    return FText::FromString(TooltipText);
}
#endif
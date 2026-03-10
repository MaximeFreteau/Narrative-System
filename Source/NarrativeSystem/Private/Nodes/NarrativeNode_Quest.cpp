// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Quest.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Quest::UNarrativeNode_Quest()
{
	NodeTitle = FText::FromString("Quest");
	QuestID = FName("NewQuest");
	
	QuestInfo.QuestName = FText::FromString("New Quest");
	QuestInfo.QuestDescription = FText::FromString("Quest description...");
	QuestInfo.QuestType = EQuestType::Side;
	QuestInfo.QuestCategory = EQuestCategory::Story;
	QuestInfo.RecommendedLevel = 1;
}

UNarrativeNode* UNarrativeNode_Quest::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
	if (!NarrativeComp)
	{
		return nullptr;
	}

	FQuestRuntimeData QuestData;
	QuestData.State = EQuestState::Active;
	QuestData.StartTime = FDateTime::Now();
	QuestData.CompletionPercentage = 0.f;

	NarrativeComp->ActiveQuestsData.Add(QuestID, QuestData);

	NarrativeComp->OnQuestStarted.Broadcast(QuestInfo.QuestName, QuestInfo.QuestDescription, QuestIcon);

	return GetDefaultNextNode();
}

#if WITH_EDITOR
FText UNarrativeNode_Quest::GetNodeTitle() const
{
	if (!QuestInfo.QuestName.IsEmpty())
	{
		return FText::Format(FText::FromString("Quest: {0}"), QuestInfo.QuestName);
	}
	return FText::FromString("Quest");
}

FLinearColor UNarrativeNode_Quest::GetNodeTitleColor() const
{
	switch (QuestInfo.QuestType)
	{
		case EQuestType::Main:
			return FLinearColor(1.0f, 0.8f, 0.0f);
		case EQuestType::Side:
			return FLinearColor(0.7f, 0.7f, 1.0f);
		case EQuestType::Daily:
			return FLinearColor(0.5f, 1.0f, 0.5f);
		case EQuestType::Hidden:
			return FLinearColor(0.8f, 0.5f, 1.0f);
		case EQuestType::Tutorial:
			return FLinearColor(1.0f, 1.0f, 1.0f);
		default:
			return FLinearColor(1.0f, 0.8f, 0.2f);
	}
}

FText UNarrativeNode_Quest::GetNodeTooltip() const
{
	FString Tooltip = FString::Printf(TEXT("Quest: %s\n"), *QuestInfo.QuestName.ToString());
	Tooltip += FString::Printf(TEXT("Type: %s\n"), *UEnum::GetValueAsString(QuestInfo.QuestType));
	
	if (!QuestInfo.QuestDescription.IsEmpty())
	{
		Tooltip += FString::Printf(TEXT("\n%s\n"), *QuestInfo.QuestDescription.ToString());
	}
	
	Tooltip += FString::Printf(TEXT("\nRewards:\nXP: %d\nGold: %d"), Rewards.ExperiencePoints, Rewards.Gold);
	
	return FText::FromString(Tooltip);
}
#endif

// Copyright Maxime Freteau. All Rights Reserved.

#include "Core/NarrativeComponent.h"
#include "Core/NarrativeAsset.h"
#include "Core/NarrativeNode.h"

UNarrativeComponent::UNarrativeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UNarrativeComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UNarrativeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UNarrativeComponent::StartNarrative(UNarrativeAsset* Narrative)
{
    if (!Narrative)
    {
        return;
    }

    if (!Narrative->GetRootNode())
    {
        return;
    }

    CurrentNarrative = Narrative;
    CurrentNode = Narrative->GetRootNode();

    ExecuteCurrentNode();
}

void UNarrativeComponent::StopCurrentNarrative()
{
    if (bIsInDialogue)
    {
        OnDialogueEnded.Broadcast();
        bIsInDialogue = false;
    }

    CurrentNarrative = nullptr;
    CurrentNode = nullptr;
}

void UNarrativeComponent::SelectDialogueChoice(int32 ChoiceIndex)
{
    if (!bIsInDialogue)
    {
        return;
    }

    if (!CurrentDialogueInfo.AvailableChoices.IsValidIndex(ChoiceIndex))
    {
        return;
    }

    TotalChoicesMadeCount++;

    UNarrativeNode* NextNode = CurrentNode ? CurrentNode->GetOutputNode(ChoiceIndex) : nullptr;
    
    bIsInDialogue = false;
    OnDialogueEnded.Broadcast();

    TransitionToNode(NextNode);
}

void UNarrativeComponent::ContinueDialogue()
{
    if (!bIsInDialogue)
    {
        return;
    }

    UNarrativeNode* NextNode = CurrentNode ? CurrentNode->GetOutputNode(0) : nullptr;
    
    bIsInDialogue = false;
    OnDialogueEnded.Broadcast();

    TransitionToNode(NextNode);
}

void UNarrativeComponent::UpdateObjectiveProgress(FGameplayTag ObjectiveTag, int32 Amount)
{
    OnObjectiveUpdated.Broadcast(ObjectiveTag, Amount, 100);
}

void UNarrativeComponent::CompleteQuest(FName QuestID)
{
    FQuestRuntimeData* QuestData = ActiveQuestsData.Find(QuestID);
    if (QuestData)
    {
        QuestData->State = EQuestState::Completed;
        QuestData->CompletionPercentage = 100.f;
        TotalQuestsCompletedCount++;
    }
}

void UNarrativeComponent::FailQuest(FName QuestID, const FText& Reason)
{
    FQuestRuntimeData* QuestData = ActiveQuestsData.Find(QuestID);
    if (QuestData)
    {
        QuestData->State = EQuestState::Failed;
        OnQuestFailed.Broadcast(QuestID, Reason);
    }
}

void UNarrativeComponent::SetIntVariable(FName VarName, int32 Value)
{
    IntVariables.Add(VarName, Value);
}

void UNarrativeComponent::SetBoolVariable(FName VarName, bool Value)
{
    BoolVariables.Add(VarName, Value);
}

void UNarrativeComponent::SetFloatVariable(FName VarName, float Value)
{
    FloatVariables.Add(VarName, Value);
}

void UNarrativeComponent::SetStringVariable(FName VarName, const FString& Value)
{
    StringVariables.Add(VarName, Value);
}

int32 UNarrativeComponent::GetIntVariable(FName VarName) const
{
    const int32* Found = IntVariables.Find(VarName);
    return Found ? *Found : 0;
}

bool UNarrativeComponent::GetBoolVariable(FName VarName) const
{
    const bool* Found = BoolVariables.Find(VarName);
    return Found ? *Found : false;
}

float UNarrativeComponent::GetFloatVariable(FName VarName) const
{
    const float* Found = FloatVariables.Find(VarName);
    return Found ? *Found : 0.f;
}

FString UNarrativeComponent::GetStringVariable(FName VarName) const
{
    const FString* Found = StringVariables.Find(VarName);
    return Found ? *Found : FString();
}

void UNarrativeComponent::AddGameplayTags(const FGameplayTagContainer& Tags)
{
    FGameplayTagContainer AddedTags;
    
    for (const FGameplayTag& Tag : Tags)
    {
        if (!ActiveTags.HasTag(Tag))
        {
            ActiveTags.AddTag(Tag);
            AddedTags.AddTag(Tag);
        }
    }

    if (AddedTags.Num() > 0)
    {
        OnGameplayTagsChanged.Broadcast(AddedTags, FGameplayTagContainer());
    }
}

void UNarrativeComponent::RemoveGameplayTags(const FGameplayTagContainer& Tags)
{
    FGameplayTagContainer RemovedTags;
    
    for (const FGameplayTag& Tag : Tags)
    {
        if (ActiveTags.HasTag(Tag))
        {
            ActiveTags.RemoveTag(Tag);
            RemovedTags.AddTag(Tag);
        }
    }

    if (RemovedTags.Num() > 0)
    {
        OnGameplayTagsChanged.Broadcast(FGameplayTagContainer(), RemovedTags);
    }
}

bool UNarrativeComponent::HasGameplayTag(FGameplayTag Tag) const
{
    return ActiveTags.HasTag(Tag);
}

bool UNarrativeComponent::HasAllGameplayTags(const FGameplayTagContainer& Tags) const
{
    return ActiveTags.HasAll(Tags);
}

bool UNarrativeComponent::HasAnyGameplayTag(const FGameplayTagContainer& Tags) const
{
    return ActiveTags.HasAny(Tags);
}

TArray<FQuestInfo> UNarrativeComponent::GetActiveQuests() const
{
    TArray<FQuestInfo> Result;
    
    for (const auto& Pair : ActiveQuestsData)
    {
        if (Pair.Value.State == EQuestState::Active)
        {
            FQuestInfo Info = GetQuestInfo(Pair.Key);
            Result.Add(Info);
        }
    }
    
    return Result;
}

TArray<FQuestInfo> UNarrativeComponent::GetCompletedQuests() const
{
    TArray<FQuestInfo> Result;
    
    for (const auto& Pair : ActiveQuestsData)
    {
        if (Pair.Value.State == EQuestState::Completed)
        {
            FQuestInfo Info = GetQuestInfo(Pair.Key);
            Result.Add(Info);
        }
    }
    
    return Result;
}

FQuestInfo UNarrativeComponent::GetQuestInfo(FName QuestID) const
{
    FQuestInfo Info;
    
    return Info;
}

void UNarrativeComponent::NotifyEnemyKilled(FName EnemyType, FGameplayTag EnemyTag)
{
    for (auto& Pair : ActiveObjectives)
    {
        FName ObjectiveID = Pair.Key;
        FObjectiveInfo& Objective = Pair.Value;
        
        if (Objective.ObjectiveType == EObjectiveType::DefeatEnemy)
        {
            bool bMatches = false;
            
            if (Objective.EnemyType == EnemyType)
            {
                bMatches = true;
            }
            
            if (Objective.EnemyTag.IsValid() && Objective.EnemyTag.MatchesTag(EnemyTag))
            {
                bMatches = true;
            }
            
            if (bMatches)
            {
                UpdateObjectiveProgress(FGameplayTag::RequestGameplayTag(*ObjectiveID.ToString()), 1);
            }
        }
    }
}

void UNarrativeComponent::NotifyItemCollected(FName ItemID)
{
    for (auto& Pair : ActiveObjectives)
    {
        FObjectiveInfo& Objective = Pair.Value;
        
        if (Objective.ObjectiveType == EObjectiveType::CollectItem)
        {
            if (Objective.ItemID == ItemID)
            {
                UpdateObjectiveProgress(FGameplayTag::RequestGameplayTag(*Pair.Key.ToString()), 1);
            }
        }
    }
}

void UNarrativeComponent::NotifyLocationVisited(FName LocationID)
{
    for (auto& Pair : ActiveObjectives)
    {
        FObjectiveInfo& Objective = Pair.Value;
        
        if (Objective.ObjectiveType == EObjectiveType::VisitLocation)
        {
            if (Objective.LocationID == LocationID)
            {
                UpdateObjectiveProgress(FGameplayTag::RequestGameplayTag(*Pair.Key.ToString()), 999);
            }
        }
    }
}

void UNarrativeComponent::NotifyNPCTalked(FName NPCID)
{
    for (auto& Pair : ActiveObjectives)
    {
        FObjectiveInfo& Objective = Pair.Value;
        
        if (Objective.ObjectiveType == EObjectiveType::TalkToNPC)
        {
            if (Objective.NPCID == NPCID)
            {
                UpdateObjectiveProgress(FGameplayTag::RequestGameplayTag(*Pair.Key.ToString()), 1);
            }
        }
    }
}

void UNarrativeComponent::NotifyObjectInteracted(FName ObjectID)
{
    for (auto& Pair : ActiveObjectives)
    {
        FObjectiveInfo& Objective = Pair.Value;
        
        if (Objective.ObjectiveType == EObjectiveType::Interact)
        {
            if (Objective.InteractObjectID == ObjectID)
            {
                UpdateObjectiveProgress(FGameplayTag::RequestGameplayTag(*Pair.Key.ToString()), 1);
            }
        }
    }
}

TArray<FObjectiveInfo> UNarrativeComponent::GetQuestObjectives(FName QuestID) const
{
    const FQuestRuntimeData* QuestData = ActiveQuestsData.Find(QuestID);
    return QuestData ? QuestData->Objectives : TArray<FObjectiveInfo>();
}

EQuestState UNarrativeComponent::GetQuestState(FName QuestID) const
{
    const FQuestRuntimeData* QuestData = ActiveQuestsData.Find(QuestID);
    return QuestData ? QuestData->State : EQuestState::NotStarted;
}

TArray<FName> UNarrativeComponent::GetDialogueHistory(FName NPCID) const
{
    const FNPCDialogueHistory* History = NPCDialogueHistories.Find(NPCID);
    return History ? History->DialogueIDs : TArray<FName>();
}

int32 UNarrativeComponent::GetTotalQuestsCompleted() const
{
    return TotalQuestsCompletedCount;
}

int32 UNarrativeComponent::GetTotalDialoguesSeen() const
{
    return TotalDialoguesSeenCount;
}

FNarrativeStats UNarrativeComponent::GetNarrativeStats() const
{
    FNarrativeStats Stats;
    Stats.TotalQuestsCompleted = TotalQuestsCompletedCount;
    Stats.TotalDialoguesSeen = TotalDialoguesSeenCount;
    Stats.TotalChoicesMade = TotalChoicesMadeCount;
    Stats.NPCReputations = NPCReputations;
    return Stats;
}

int32 UNarrativeComponent::GetNPCReputation(FName NPCID) const
{
    const int32* Found = NPCReputations.Find(NPCID);
    return Found ? *Found : 0;
}

ENPCRelationship UNarrativeComponent::GetNPCRelationshipLevel(FName NPCID) const
{
    int32 Rep = GetNPCReputation(NPCID);
    
    if (Rep < -50) return ENPCRelationship::Hostile;
    if (Rep < 0) return ENPCRelationship::Unfriendly;
    if (Rep < 50) return ENPCRelationship::Neutral;
    if (Rep < 100) return ENPCRelationship::Friendly;
    return ENPCRelationship::Allied;
}

void UNarrativeComponent::ModifyNPCReputation(FName NPCID, int32 Amount)
{
    int32 CurrentRep = GetNPCReputation(NPCID);
    NPCReputations.Add(NPCID, CurrentRep + Amount);
}

void UNarrativeComponent::ExecuteCurrentNode()
{
    if (!CurrentNode)
    {
        StopCurrentNarrative();
        return;
    }

    if (!CurrentNode->CanExecute(this))
    {
        StopCurrentNarrative();
        return;
    }

    UNarrativeNode* NextNode = CurrentNode->Execute(this);
    
    if (NextNode != CurrentNode)
    {
        TransitionToNode(NextNode);
    }
}

void UNarrativeComponent::TransitionToNode(UNarrativeNode* NextNode)
{
    if (CurrentNode)
    {
        CurrentNode->OnNodeCompleted(this);
    }

    CurrentNode = NextNode;

    if (CurrentNode)
    {
        ExecuteCurrentNode();
    }
    else
    {
        StopCurrentNarrative();
    }
}
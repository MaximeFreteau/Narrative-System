// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NarrativeTypes.h"
#include "NarrativeComponent.generated.h"

class UNarrativeAsset;
class UNarrativeNode;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, const TArray<FDialogueLine>&, Lines, const TArray<FDialogueChoice>&, Choices);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDialogueLineDisplayed, FName, SpeakerID, FText, SpeakerName, FText, Text, UTexture2D*, Portrait);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoicesPresented, const TArray<FDialogueChoice>&, Choices);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestStarted, FText, QuestName, FText, Description, UTexture2D*, Icon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnQuestUpdated, FName, QuestID, FText, ObjectiveText, int32, Current, int32, Required);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompleted, FName, QuestID, FNarrativeReward, Rewards);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestFailed, FName, QuestID, FText, FailReason);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveStarted, FText, ObjectiveText, bool, bOptional);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, FText, ObjectiveText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnObjectiveUpdated, FGameplayTag, ObjectiveTag, int32, Progress, int32, Required);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnNotification, FText, Title, FText, Message, ENotificationType, Type, float, Duration);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayTagsChanged, FGameplayTagContainer, AddedTags, FGameplayTagContainer, RemovedTags);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRewardReceived, int32, XP, int32, Gold, const TArray<FItemReward>&, Items);

UCLASS(ClassGroup=(Narrative), meta=(BlueprintSpawnableComponent))
class NARRATIVESYSTEM_API UNarrativeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNarrativeComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnDialogueLineDisplayed OnDialogueLineDisplayed;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnChoicesPresented OnChoicesPresented;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnDialogueEnded OnDialogueEnded;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnQuestStarted OnQuestStarted;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnQuestUpdated OnQuestUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnQuestCompleted OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnQuestFailed OnQuestFailed;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnObjectiveStarted OnObjectiveStarted;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnObjectiveCompleted OnObjectiveCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnObjectiveUpdated OnObjectiveUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnNotification OnNotification;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
    FOnRewardReceived OnRewardReceived;

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void StartNarrative(UNarrativeAsset* Narrative);

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void StopCurrentNarrative();

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void SelectDialogueChoice(int32 ChoiceIndex);

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void ContinueDialogue();

    UFUNCTION(BlueprintCallable, Category = "Narrative|Quests")
    void UpdateObjectiveProgress(FGameplayTag ObjectiveTag, int32 Amount = 1);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Quests")
    void CompleteQuest(FName QuestID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Quests")
    void FailQuest(FName QuestID, const FText& Reason);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    void SetIntVariable(FName VarName, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    void SetBoolVariable(FName VarName, bool Value);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    void SetFloatVariable(FName VarName, float Value);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    void SetStringVariable(FName VarName, const FString& Value);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    int32 GetIntVariable(FName VarName) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    bool GetBoolVariable(FName VarName) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    float GetFloatVariable(FName VarName) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Variables")
    FString GetStringVariable(FName VarName) const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative|Tags")
    FGameplayTagContainer ActiveTags;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Tags")
    void AddGameplayTags(const FGameplayTagContainer& Tags);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Tags")
    void RemoveGameplayTags(const FGameplayTagContainer& Tags);

    UFUNCTION(BlueprintPure, Category = "Narrative|Tags")
    bool HasGameplayTag(FGameplayTag Tag) const;

    UFUNCTION(BlueprintPure, Category = "Narrative|Tags")
    bool HasAllGameplayTags(const FGameplayTagContainer& Tags) const;

    UFUNCTION(BlueprintPure, Category = "Narrative|Tags")
    bool HasAnyGameplayTag(const FGameplayTagContainer& Tags) const;

    UFUNCTION(BlueprintPure, Category = "Narrative|Tags")
    FGameplayTagContainer GetActiveGameplayTags() const { return ActiveTags; }

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Tags")
    FOnGameplayTagsChanged OnGameplayTagsChanged;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    TArray<FQuestInfo> GetActiveQuests() const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    TArray<FQuestInfo> GetCompletedQuests() const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    FQuestInfo GetQuestInfo(FName QuestID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    TArray<FObjectiveInfo> GetQuestObjectives(FName QuestID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    EQuestState GetQuestState(FName QuestID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    bool IsInDialogue() const { return bIsInDialogue; }

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    FDialogueInfo GetCurrentDialogue() const { return CurrentDialogueInfo; }

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    TArray<FName> GetDialogueHistory(FName NPCID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    int32 GetTotalQuestsCompleted() const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    int32 GetTotalDialoguesSeen() const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    FNarrativeStats GetNarrativeStats() const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    int32 GetNPCReputation(FName NPCID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    ENPCRelationship GetNPCRelationshipLevel(FName NPCID) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Query")
    void ModifyNPCReputation(FName NPCID, int32 Amount);

    UPROPERTY(BlueprintReadOnly, Category = "Narrative|Quests")
    TMap<FName, FObjectiveInfo> ActiveObjectives;

    UPROPERTY(BlueprintReadOnly, Category = "Narrative|Quests")
    TMap<FName, int32> ObjectiveProgress;

    UPROPERTY(BlueprintReadOnly, Category = "Narrative|Quests")
    TMap<FName, FQuestInfo> ActiveQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Narrative|Quests")
    TMap<FName, EQuestState> QuestStates;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Objectives")
    void NotifyEnemyKilled(FName EnemyType, FGameplayTag EnemyTag);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Objectives")
    void NotifyItemCollected(FName ItemID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Objectives")
    void NotifyLocationVisited(FName LocationID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Objectives")
    void NotifyNPCTalked(FName NPCID);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Objectives")
    void NotifyObjectInteracted(FName ObjectID);

protected:
    UPROPERTY()
    TObjectPtr<UNarrativeAsset> CurrentNarrative;

    UPROPERTY()
    TObjectPtr<UNarrativeNode> CurrentNode;

    UPROPERTY()
    bool bIsInDialogue = false;

    UPROPERTY()
    FDialogueInfo CurrentDialogueInfo;

    UPROPERTY()
    TMap<FName, FQuestRuntimeData> ActiveQuestsData;

    UPROPERTY()
    TMap<FName, int32> IntVariables;

    UPROPERTY()
    TMap<FName, bool> BoolVariables;

    UPROPERTY()
    TMap<FName, float> FloatVariables;

    UPROPERTY()
    TMap<FName, FString> StringVariables;

    UPROPERTY()
    TMap<FName, int32> NPCReputations;

    UPROPERTY()
    int32 TotalQuestsCompletedCount = 0;

    UPROPERTY()
    int32 TotalDialoguesSeenCount = 0;

    UPROPERTY()
    int32 TotalChoicesMadeCount = 0;

    UPROPERTY()
    TMap<FName, FNPCDialogueHistory> NPCDialogueHistories;

    void ExecuteCurrentNode();
    void TransitionToNode(UNarrativeNode* NextNode);

    friend class UNarrativeNode;
    friend class UNarrativeNode_Dialogue;
    friend class UNarrativeNode_Quest;
    friend class UNarrativeNode_Objective;
    friend class UNarrativeSaveSystem;
};
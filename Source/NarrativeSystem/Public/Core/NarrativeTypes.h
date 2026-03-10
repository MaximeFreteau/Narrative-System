// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/Texture2D.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimMontage.h"
#include "NarrativeTypes.generated.h"

UENUM(BlueprintType)
enum class ENarrativeType : uint8
{
    Lore UMETA(DisplayName = "Lore/Dialogue"),
    Quest UMETA(DisplayName = "Quest"),
    Questline UMETA(DisplayName = "Questline")
};

UENUM(BlueprintType)
enum class EQuestState : uint8
{
    NotStarted UMETA(DisplayName = "Not Started"),
    Active UMETA(DisplayName = "Active"),
    Completed UMETA(DisplayName = "Completed"),
    Failed UMETA(DisplayName = "Failed")
};

UENUM(BlueprintType)
enum class EDialogueContinueMode : uint8
{
    Manual UMETA(DisplayName = "Manual (Click to continue)"),
    Auto UMETA(DisplayName = "Auto (Timer)"),
    WaitForAudio UMETA(DisplayName = "Wait for audio to finish")
};

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
    DefeatEnemy         UMETA(DisplayName = "Defeat Enemy"),
    CollectItem         UMETA(DisplayName = "Collect Item"),
    TalkToNPC           UMETA(DisplayName = "Talk to NPC"),
    VisitLocation       UMETA(DisplayName = "Visit Location"),
    Escort              UMETA(DisplayName = "Escort NPC"),
    Defend              UMETA(DisplayName = "Defend Location"),
    Survive             UMETA(DisplayName = "Survive for Duration"),
    Interact            UMETA(DisplayName = "Interact with Object"),
    Craft               UMETA(DisplayName = "Craft Item"),
    Deliver             UMETA(DisplayName = "Deliver Item"),
    Custom              UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EConditionType : uint8
{
    HasTag          UMETA(DisplayName = "Has Gameplay Tag"),
    HasAllTags      UMETA(DisplayName = "Has All Tags"),
    HasAnyTag       UMETA(DisplayName = "Has Any Tag"),
    Simple          UMETA(DisplayName = "Simple Condition"),
    Complex         UMETA(DisplayName = "Complex Condition"),
    CompareInt      UMETA(DisplayName = "Compare Integer"),
    CompareBool     UMETA(DisplayName = "Compare Boolean"),
    CompareFloat    UMETA(DisplayName = "Compare Float"),
    VariableCheck   UMETA(DisplayName = "Variable Check"),
    CompareString   UMETA(DisplayName = "Compare String"),
    CheckQuestState UMETA(DisplayName = "Check Quest State"),
    ReputationCheck UMETA(DisplayName = "Reputation Check"),
    PlayerLevel     UMETA(DisplayName = "Player Level"),
    Custom          UMETA(DisplayName = "Custom Condition")
};

UENUM(BlueprintType)
enum class EComparisonOp : uint8
{
    Equal UMETA(DisplayName = "=="),
    NotEqual UMETA(DisplayName = "!="),
    Less UMETA(DisplayName = "<"),
    LessOrEqual UMETA(DisplayName = "<="),
    Greater UMETA(DisplayName = ">"),
    GreaterOrEqual UMETA(DisplayName = ">=")
};

UENUM(BlueprintType)
enum class ELogicalOp : uint8
{
    And UMETA(DisplayName = "AND"),
    Or UMETA(DisplayName = "OR")
};

UENUM(BlueprintType)
enum class ESetOperation : uint8
{
    Set UMETA(DisplayName = "Set (=)"),
    Add UMETA(DisplayName = "Add (+=)"),
    Subtract UMETA(DisplayName = "Subtract (-=)"),
    Multiply UMETA(DisplayName = "Multiply (*=)"),
    Divide UMETA(DisplayName = "Divide (/=)")
};

UENUM(BlueprintType)
enum class EVariableType : uint8
{
    Int UMETA(DisplayName = "Integer"),
    Float UMETA(DisplayName = "Float"),
    Bool UMETA(DisplayName = "Boolean"),
    String UMETA(DisplayName = "String")
};

UENUM(BlueprintType)
enum class ENotificationType : uint8
{
    Info UMETA(DisplayName = "Info"),
    Success UMETA(DisplayName = "Success"),
    Warning UMETA(DisplayName = "Warning"),
    Error UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ENPCRelationship : uint8
{
    Hostile UMETA(DisplayName = "Hostile"),
    Unfriendly UMETA(DisplayName = "Unfriendly"),
    Neutral UMETA(DisplayName = "Neutral"),
    Friendly UMETA(DisplayName = "Friendly"),
    Allied UMETA(DisplayName = "Allied")
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
    Main        UMETA(DisplayName = "Main Quest"),
    Side        UMETA(DisplayName = "Side Quest (Favor)"),
    Daily       UMETA(DisplayName = "Daily Quest"),
    Hidden      UMETA(DisplayName = "Hidden Quest"),
    Tutorial    UMETA(DisplayName = "Tutorial")
};

UENUM(BlueprintType)
enum class EQuestCategory : uint8
{
    Story       UMETA(DisplayName = "Story"),
    Combat      UMETA(DisplayName = "Combat"),
    Exploration UMETA(DisplayName = "Exploration"),
    Collection  UMETA(DisplayName = "Collection"),
    Social      UMETA(DisplayName = "Social"),
    Puzzle      UMETA(DisplayName = "Puzzle")
};

USTRUCT(BlueprintType)
struct FQuestInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (MultiLine = true))
    FText QuestDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (MultiLine = true))
    FText QuestLongDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestType QuestType = EQuestType::Side;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestCategory QuestCategory = EQuestCategory::Story;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ClampMin = "1"))
    int32 RecommendedLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TObjectPtr<UTexture2D> QuestIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName QuestRegion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FGameplayTagContainer QuestTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest|Requirements")
    TArray<FName> RequiredQuests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest|Requirements")
    FGameplayTagContainer RequiredTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool bRepeatable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ClampMin = "0"))
    float TimeLimit = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool bCanFail = false;
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FDialogueLine
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName SpeakerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText SpeakerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (MultiLine = true))
    FText DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TObjectPtr<UTexture2D> Portrait;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TObjectPtr<USoundBase> VoiceAudio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TObjectPtr<UAnimMontage> Animation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FGameplayTagContainer DialogueTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    float DelayBeforeNext = 0.f;

    FDialogueLine()
        : SpeakerID(NAME_None)
        , SpeakerName(FText::GetEmpty())
        , DialogueText(FText::GetEmpty())
        , Portrait(nullptr)
        , VoiceAudio(nullptr)
        , Animation(nullptr)
        , DelayBeforeNext(0.f)
    {}
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FDialogueChoice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText ChoiceText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FGameplayTagQuery Condition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice")
    FGameplayTagContainer RequiredTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice")
    FGameplayTagContainer BlockedByTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Choice")
    bool bHideIfRequirementsNotMet = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bHidden = false;

    FDialogueChoice()
        : ChoiceText(FText::GetEmpty())
        , bHidden(false)
    {}
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FDialogueInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
    TArray<FDialogueLine> CurrentLines;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
    TArray<FDialogueChoice> AvailableChoices;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
    int32 CurrentLineIndex = 0;
};

USTRUCT(BlueprintType)
struct FConditionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    EConditionType Type = EConditionType::Simple;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    FName VariableName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    int32 CompareValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    EComparisonOp ComparisonOp = EComparisonOp::Equal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    ELogicalOp LogicalOp = ELogicalOp::And;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
    FGameplayTagContainer TagQuery;
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FItemReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    FGameplayTag ItemTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    int32 Quantity = 1;

    FItemReward()
        : Quantity(1)
    {}
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FNarrativeReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    int32 ExperiencePoints = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    int32 Gold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    TArray<FItemReward> Items;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    FGameplayTagContainer UnlockTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    FGameplayTagContainer RemoveTags; 

    FNarrativeReward()
        : ExperiencePoints(0)
        , Gold(0)
    {}
};

USTRUCT(BlueprintType)
struct FObjectiveInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FText ObjectiveDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    EObjectiveType ObjectiveType = EObjectiveType::Custom;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|DefeatEnemy", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::DefeatEnemy", EditConditionHides))
    FName EnemyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|DefeatEnemy", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::DefeatEnemy", EditConditionHides, ClampMin = "1"))
    int32 EnemyCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|DefeatEnemy", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::DefeatEnemy", EditConditionHides))
    FGameplayTag EnemyTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|CollectItem", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::CollectItem", EditConditionHides))
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|CollectItem", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::CollectItem", EditConditionHides))
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|CollectItem", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::CollectItem", EditConditionHides, ClampMin = "1"))
    int32 ItemCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|CollectItem", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::CollectItem", EditConditionHides))
    TObjectPtr<UTexture2D> ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|TalkToNPC", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::TalkToNPC", EditConditionHides))
    FName NPCID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|TalkToNPC", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::TalkToNPC", EditConditionHides))
    FText NPCName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|TalkToNPC", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::TalkToNPC", EditConditionHides))
    FName NPCLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|VisitLocation", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::VisitLocation", EditConditionHides))
    FName LocationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|VisitLocation", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::VisitLocation", EditConditionHides))
    FText LocationName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|VisitLocation", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::VisitLocation", EditConditionHides))
    FVector LocationPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|VisitLocation", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::VisitLocation", EditConditionHides, ClampMin = "0.0"))
    float DetectionRadius = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Escort", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Escort", EditConditionHides))
    FName EscortNPCID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Escort", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Escort", EditConditionHides))
    FVector EscortDestination;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Escort", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Escort", EditConditionHides))
    bool bNPCCanDie = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Defend", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Defend", EditConditionHides))
    FVector DefendPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Defend", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Defend", EditConditionHides))
    float DefendRadius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Defend", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Defend", EditConditionHides))
    float DefendDuration = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Survive", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Survive", EditConditionHides))
    float SurviveDuration = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Interact", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Interact", EditConditionHides))
    FName InteractObjectID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Interact", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Interact", EditConditionHides))
    FText InteractObjectName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Interact", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Interact", EditConditionHides))
    int32 InteractCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Craft", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Craft", EditConditionHides))
    FName CraftItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Craft", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Craft", EditConditionHides))
    int32 CraftCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Deliver", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Deliver", EditConditionHides))
    FName DeliverItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Deliver", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Deliver", EditConditionHides))
    FName DeliverToNPCID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Deliver", 
        meta = (EditCondition = "ObjectiveType == EObjectiveType::Deliver", EditConditionHides))
    FText DeliverToNPCName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bIsOptional = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bIsHidden = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FGameplayTagContainer ObjectiveTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|UI")
    TObjectPtr<UTexture2D> MarkerIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|UI")
    FLinearColor MarkerColor = FLinearColor::Yellow;
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FNarrativeStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 TotalQuestsCompleted = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 TotalDialoguesSeen = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 TotalChoicesMade = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float TotalPlaytime = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    TMap<FName, int32> NPCReputations;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    TArray<FName> UnlockedLore;
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FQuestRuntimeData
{
    GENERATED_BODY()

    UPROPERTY()
    EQuestState State = EQuestState::NotStarted;

    UPROPERTY()
    TArray<FObjectiveInfo> Objectives;

    UPROPERTY()
    FDateTime StartTime;

    UPROPERTY()
    float CompletionPercentage = 0.f;

    FQuestRuntimeData()
        : State(EQuestState::NotStarted)
        , CompletionPercentage(0.f)
    {}
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FNPCDialogueHistory
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FName> DialogueIDs;

    UPROPERTY()
    int32 TotalInteractions = 0;

    UPROPERTY()
    FDateTime LastInteractionTime;

    FNPCDialogueHistory()
        : TotalInteractions(0)
    {}
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FNarrativeMetadata
{
    GENERATED_BODY()

    UPROPERTY()
    FName AssetID;

    UPROPERTY()
    ENarrativeType Type = ENarrativeType::Lore;

    UPROPERTY()
    int32 MinLevel = 0;

    UPROPERTY()
    int32 MaxLevel = 999;

    UPROPERTY()
    TArray<FName> Prerequisites;

    UPROPERTY()
    FGameplayTag RegionTag;

    UPROPERTY()
    FVector WorldLocation = FVector::ZeroVector;

    UPROPERTY()
    FGameplayTagContainer AssetTags;

    UPROPERTY()
    FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FNarrativeAssetReference
{
    GENERATED_BODY()

    UPROPERTY()
    TSoftObjectPtr<class UNarrativeAsset> AssetPtr;

    UPROPERTY()
    FNarrativeMetadata Metadata;
};

USTRUCT()
struct NARRATIVESYSTEM_API FActiveObjectiveData
{
    GENERATED_BODY()

    UPROPERTY()
    FName ObjectiveID;

    UPROPERTY()
    int32 Progress = 0;
};

USTRUCT()
struct NARRATIVESYSTEM_API FNarrativeSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<uint8> CompletedQuestsBitfield;

    UPROPERTY()
    TMap<FName, int32> ModifiedIntVars;

    UPROPERTY()
    TMap<FName, bool> ModifiedBoolVars;

    UPROPERTY()
    TMap<FName, float> ModifiedFloatVars;

    UPROPERTY()
    TMap<FName, FString> ModifiedStringVars;

    UPROPERTY()
    TArray<FActiveObjectiveData> ActiveObjectives;

    UPROPERTY()
    TArray<FName> RecentDialogues;

    UPROPERTY()
    TMap<FName, int32> NPCReputations;

    UPROPERTY()
    FDateTime SaveTime;

    UPROPERTY()
    int32 SaveVersion = 1;
};
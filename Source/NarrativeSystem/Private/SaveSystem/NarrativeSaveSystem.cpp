// Copyright Maxime. All Rights Reserved.

#include "SaveSystem/NarrativeSaveSystem.h"
#include "SaveSystem/NarrativeSaveGame.h"
#include "Core/NarrativeComponent.h"
#include "Core/NarrativeSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UNarrativeSaveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const UNarrativeSettings* Settings = GetDefault<UNarrativeSettings>();
    if (Settings)
    {
        bUseDeltaCompression = Settings->bUseDeltaCompression;
        bUseIncrementalSaves = Settings->bUseIncrementalSaves;
        AutoSaveInterval = Settings->AutoSaveInterval;
        bDebugLogging = Settings->bEnableDebugLogging;
    }
}

void UNarrativeSaveSystem::Deinitialize()
{
    StopAutoSave();
    Super::Deinitialize();
}

bool UNarrativeSaveSystem::SaveGame(const FString& SlotName, int32 UserIndex)
{
    UNarrativeComponent* NarrativeComp = FindNarrativeComponent();
    if (!NarrativeComp)
    {
        return false;
    }

    if (!CurrentSaveGame)
    {
        CurrentSaveGame = Cast<UNarrativeSaveGame>(UGameplayStatics::CreateSaveGameObject(UNarrativeSaveGame::StaticClass()));
    }

    if (!CurrentSaveGame)
    {
        return false;
    }

    CurrentSaveGame->SlotName = SlotName;
    CurrentSaveGame->UserIndex = UserIndex;
    CurrentSaveGame->SaveTime = FDateTime::Now();
    CurrentSaveGame->SaveVersion = 1;

    CollectNarrativeData(NarrativeComp, CurrentSaveGame->NarrativeData);

    bool bSuccess = SaveGameInternal(CurrentSaveGame, SlotName, UserIndex);

    if (bSuccess)
    {
        LastSavedState = CurrentSaveGame->NarrativeData;
        bIsDirty = false;
        DirtyQuests.Empty();
        DirtyVariables.Empty();

        if (bDebugLogging)
        {
            int32 Size = CurrentSaveGame->GetApproximateSize();
        }
    }

    OnSaveCompleted.Broadcast(bSuccess, SlotName);
    return bSuccess;
}

bool UNarrativeSaveSystem::SaveGameIncremental(const FString& SlotName, int32 UserIndex)
{
    if (!bUseIncrementalSaves)
    {
        return SaveGame(SlotName, UserIndex);
    }

    if (!bIsDirty)
    {
        return true;
    }
    
    return SaveGame(SlotName, UserIndex);
}

void UNarrativeSaveSystem::SaveGameAsync(const FString& SlotName, int32 UserIndex)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, SlotName, UserIndex]()
    {
        bool bSuccess = SaveGame(SlotName, UserIndex);
        
        AsyncTask(ENamedThreads::GameThread, [this, bSuccess, SlotName]()
        {
            OnSaveCompleted.Broadcast(bSuccess, SlotName);
        });
    });
}

bool UNarrativeSaveSystem::LoadGame(const FString& SlotName, int32 UserIndex)
{
    if (!DoesSaveGameExist(SlotName, UserIndex))
    {
        OnLoadCompleted.Broadcast(false, SlotName);
        return false;
    }

    bool bSuccess = LoadGameInternal(SlotName, UserIndex);

    OnLoadCompleted.Broadcast(bSuccess, SlotName);
    return bSuccess;
}

void UNarrativeSaveSystem::LoadGameAsync(const FString& SlotName, int32 UserIndex)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, SlotName, UserIndex]()
    {
        bool bSuccess = LoadGame(SlotName, UserIndex);
        
        AsyncTask(ENamedThreads::GameThread, [this, bSuccess, SlotName]()
        {
            OnLoadCompleted.Broadcast(bSuccess, SlotName);
        });
    });
}

bool UNarrativeSaveSystem::DoesSaveGameExist(const FString& SlotName, int32 UserIndex) const
{
    return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

bool UNarrativeSaveSystem::DeleteSaveGame(const FString& SlotName, int32 UserIndex)
{
    return UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
}

UNarrativeSaveGame* UNarrativeSaveSystem::GetSaveGameInfo(const FString& SlotName, int32 UserIndex)
{
    USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
    return Cast<UNarrativeSaveGame>(SaveGame);
}

TArray<FString> UNarrativeSaveSystem::GetAllSaveSlots() const
{
    TArray<FString> Slots;
    if (DoesSaveGameExist(TEXT("NarrativeSave"), 0))
    {
        Slots.Add(TEXT("NarrativeSave"));
    }
    return Slots;
}

void UNarrativeSaveSystem::SetAutoSaveEnabled(bool bEnabled)
{
    if (bEnabled == bAutoSaveEnabled)
    {
        return;
    }

    bAutoSaveEnabled = bEnabled;

    if (bEnabled)
    {
        StartAutoSave();
    }
    else
    {
        StopAutoSave();
    }
}

float UNarrativeSaveSystem::GetTimeUntilNextAutoSave() const
{
    if (!bAutoSaveEnabled || !GetWorld())
    {
        return -1.f;
    }

    return GetWorld()->GetTimerManager().GetTimerRemaining(AutoSaveTimerHandle);
}

void UNarrativeSaveSystem::StartAutoSave()
{
    if (!GetWorld())
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(
        AutoSaveTimerHandle,
        this,
        &UNarrativeSaveSystem::OnAutoSaveTimer,
        AutoSaveInterval,
        true
    );
}

void UNarrativeSaveSystem::StopAutoSave()
{
    if (!GetWorld())
    {
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
}

void UNarrativeSaveSystem::OnAutoSaveTimer()
{

    SaveGameAsync(TEXT("NarrativeSave_Auto"), 0);
}

bool UNarrativeSaveSystem::SaveGameInternal(UNarrativeSaveGame* SaveGame, const FString& SlotName, int32 UserIndex)
{
    return UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
}

bool UNarrativeSaveSystem::LoadGameInternal(const FString& SlotName, int32 UserIndex)
{
    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
    UNarrativeSaveGame* NarrativeSave = Cast<UNarrativeSaveGame>(LoadedGame);

    if (!NarrativeSave)
    {
        return false;
    }

    CurrentSaveGame = NarrativeSave;

    UNarrativeComponent* NarrativeComp = FindNarrativeComponent();
    if (NarrativeComp)
    {
        ApplyNarrativeData(NarrativeComp, NarrativeSave->NarrativeData);
    }

    LastSavedState = NarrativeSave->NarrativeData;
    bIsDirty = false;

    return true;
}

void UNarrativeSaveSystem::CollectNarrativeData(UNarrativeComponent* NarrativeComp, FNarrativeSaveData& OutData)
{
    if (!NarrativeComp)
    {
        return;
    }

    OutData.ModifiedIntVars = NarrativeComp->IntVariables;
    OutData.ModifiedBoolVars = NarrativeComp->BoolVariables;
    OutData.ModifiedFloatVars = NarrativeComp->FloatVariables;
    OutData.ModifiedStringVars = NarrativeComp->StringVariables;

    OutData.ActiveObjectives.Empty();
    for (const auto& Pair : NarrativeComp->ActiveQuestsData)
    {
        for (const FObjectiveInfo& Objective : Pair.Value.Objectives)
        {
            FActiveObjectiveData ObjData;
            ObjData.ObjectiveID = FName(*Objective.ObjectiveDescription.ToString());
            ObjData.Progress = 0;
            OutData.ActiveObjectives.Add(ObjData);
        }
    }
    
    OutData.RecentDialogues.Empty();
    for (const auto& Pair : NarrativeComp->NPCDialogueHistories)
    {
        for (const FName& DialogueID : Pair.Value.DialogueIDs)
        {
            OutData.RecentDialogues.Add(DialogueID);
            if (OutData.RecentDialogues.Num() >= 100)
            {
                break;
            }
        }
        if (OutData.RecentDialogues.Num() >= 100)
        {
            break;
        }
    }

    OutData.NPCReputations = NarrativeComp->NPCReputations;

    OutData.SaveTime = FDateTime::Now();
    OutData.SaveVersion = 1;
}

void UNarrativeSaveSystem::ApplyNarrativeData(UNarrativeComponent* NarrativeComp, const FNarrativeSaveData& Data)
{
    if (!NarrativeComp)
    {
        return;
    }

    NarrativeComp->IntVariables = Data.ModifiedIntVars;
    NarrativeComp->BoolVariables = Data.ModifiedBoolVars;
    NarrativeComp->FloatVariables = Data.ModifiedFloatVars;
    NarrativeComp->StringVariables = Data.ModifiedStringVars;

    NarrativeComp->NPCReputations = Data.NPCReputations;
}

void UNarrativeSaveSystem::CompressQuestData(const TMap<FName, EQuestState>& QuestStates, TArray<uint8>& OutBitfield)
{
}

void UNarrativeSaveSystem::DecompressQuestData(const TArray<uint8>& Bitfield, TMap<FName, EQuestState>& OutQuestStates)
{
}

FNarrativeSaveData UNarrativeSaveSystem::CreateDeltaSave(const FNarrativeSaveData& Current, const FNarrativeSaveData& Previous)
{
    FNarrativeSaveData Delta;

    return Current;
}

UNarrativeComponent* UNarrativeSaveSystem::FindNarrativeComponent() const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC || !PC->GetPawn())
    {
        return nullptr;
    }

    return PC->GetPawn()->FindComponentByClass<UNarrativeComponent>();
}
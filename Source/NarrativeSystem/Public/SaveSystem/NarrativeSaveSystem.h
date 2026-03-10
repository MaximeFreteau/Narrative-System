// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeSaveSystem.generated.h"

class UNarrativeSaveGame;
class UNarrativeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveCompleted, bool, bSuccess, const FString&, SlotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadCompleted, bool, bSuccess, const FString&, SlotName);

UCLASS()
class NARRATIVESYSTEM_API UNarrativeSaveSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Save")
    FOnSaveCompleted OnSaveCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Narrative|Save")
    FOnLoadCompleted OnLoadCompleted;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    bool SaveGame(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    bool SaveGameIncremental(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    void SaveGameAsync(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    bool LoadGame(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    void LoadGameAsync(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    bool DoesSaveGameExist(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0) const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    bool DeleteSaveGame(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    UNarrativeSaveGame* GetSaveGameInfo(const FString& SlotName = TEXT("NarrativeSave"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    TArray<FString> GetAllSaveSlots() const;

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    void SetAutoSaveEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Narrative|Save")
    float GetTimeUntilNextAutoSave() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    bool bUseDeltaCompression = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    bool bUseIncrementalSaves = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    float AutoSaveInterval = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugLogging = false;

protected:
    
    UPROPERTY()
    TObjectPtr<UNarrativeSaveGame> CurrentSaveGame;

    UPROPERTY()
    FNarrativeSaveData LastSavedState;

    bool bIsDirty = false;
    TSet<FName> DirtyQuests;
    TSet<FName> DirtyVariables;

    FTimerHandle AutoSaveTimerHandle;
    bool bAutoSaveEnabled = false;

private:

    bool SaveGameInternal(UNarrativeSaveGame* SaveGame, const FString& SlotName, int32 UserIndex);
    bool LoadGameInternal(const FString& SlotName, int32 UserIndex);
    
    void CollectNarrativeData(UNarrativeComponent* NarrativeComp, FNarrativeSaveData& OutData);
    void ApplyNarrativeData(UNarrativeComponent* NarrativeComp, const FNarrativeSaveData& Data);
    
    void CompressQuestData(const TMap<FName, EQuestState>& QuestStates, TArray<uint8>& OutBitfield);
    void DecompressQuestData(const TArray<uint8>& Bitfield, TMap<FName, EQuestState>& OutQuestStates);
    
    FNarrativeSaveData CreateDeltaSave(const FNarrativeSaveData& Current, const FNarrativeSaveData& Previous);
    
    void OnAutoSaveTimer();
    void StartAutoSave();
    void StopAutoSave();
    
    UNarrativeComponent* FindNarrativeComponent() const;
};
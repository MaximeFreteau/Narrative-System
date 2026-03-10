// Copyright Maxime Freteau. All Rights Reserved.

#include "SaveSystem/NarrativeSaveGame.h"

UNarrativeSaveGame::UNarrativeSaveGame()
{
	SlotName = TEXT("NarrativeSave");
	UserIndex = 0;
	SaveVersion = 1;
	PlayerLevel = 1;
	TotalPlaytime = 0.f;
}

int32 UNarrativeSaveGame::GetApproximateSize() const
{
	int32 Size = 0;

	Size += 100;

	Size += NarrativeData.CompletedQuestsBitfield.Num();

	Size += NarrativeData.ModifiedIntVars.Num() * 20;
	Size += NarrativeData.ModifiedBoolVars.Num() * 20;
	Size += NarrativeData.ModifiedFloatVars.Num() * 20;
	Size += NarrativeData.ModifiedStringVars.Num() * 50;

	Size += NarrativeData.ActiveObjectives.Num() * 20;

	Size += NarrativeData.RecentDialogues.Num() * 10;

	Size += NarrativeData.NPCReputations.Num() * 20;

	return Size;
}

void UNarrativeSaveGame::ResetData()
{
	NarrativeData = FNarrativeSaveData();
	PlayerLevel = 1;
	TotalPlaytime = 0.f;
	SaveTime = FDateTime::Now();
}
// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeSaveGame.generated.h"

UCLASS()
class NARRATIVESYSTEM_API UNarrativeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UNarrativeSaveGame();

	UPROPERTY()
	FString SlotName;

	UPROPERTY()
	int32 UserIndex = 0;

	UPROPERTY()
	FDateTime SaveTime;

	UPROPERTY()
	int32 SaveVersion = 1;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	float TotalPlaytime = 0.f;

	UPROPERTY()
	FNarrativeSaveData NarrativeData;

	int32 GetApproximateSize() const;

	void ResetData();
};
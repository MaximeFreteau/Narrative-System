// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeNode_Quest.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Quest : public UNarrativeNode
{
	GENERATED_BODY()

public:
	UNarrativeNode_Quest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FQuestInfo QuestInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TObjectPtr<UTexture2D> QuestIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bAllowParallelObjectives = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FNarrativeReward Rewards;

	virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTooltip() const override;
#endif
};
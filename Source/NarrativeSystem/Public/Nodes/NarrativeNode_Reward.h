// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeNode_Reward.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Reward : public UNarrativeNode
{
	GENERATED_BODY()

public:
	UNarrativeNode_Reward();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FNarrativeReward Rewards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	bool bShowNotification = true;

	virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTooltip() const override;
#endif
};
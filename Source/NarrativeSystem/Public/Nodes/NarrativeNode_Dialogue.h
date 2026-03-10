// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeNode_Dialogue.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Dialogue : public UNarrativeNode
{
	GENERATED_BODY()

public:
	UNarrativeNode_Dialogue();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FDialogueLine> Lines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FDialogueChoice> Choices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EDialogueContinueMode ContinueMode = EDialogueContinueMode::Manual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (EditCondition = "ContinueMode == EDialogueContinueMode::Auto", EditConditionHides))
	float AutoContinueDelay = 3.0f;

	virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;
	virtual bool CanExecute_Implementation(UNarrativeComponent* NarrativeComp) const override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTooltip() const override;
#endif

private:
	void DisplayDialogue(UNarrativeComponent* NarrativeComp);
	void DisplayChoices(UNarrativeComponent* NarrativeComp);
};
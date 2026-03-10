// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "NarrativeNode_Branch.generated.h"

class UNarrativeNode_Condition;

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Branch : public UNarrativeNode
{
	GENERATED_BODY()

public:
	UNarrativeNode_Branch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Branch")
	TObjectPtr<UNarrativeNode_Condition> Condition;

	virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTooltip() const override;
#endif
};
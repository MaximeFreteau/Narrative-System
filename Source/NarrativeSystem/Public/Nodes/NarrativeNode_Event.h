// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeNode_Event.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Event : public UNarrativeNode
{
	GENERATED_BODY()

public:
	UNarrativeNode_Event();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TMap<FName, int32> IntParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TMap<FName, FString> StringParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TMap<FName, bool> BoolParameters;

	virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTooltip() const override;
#endif
};
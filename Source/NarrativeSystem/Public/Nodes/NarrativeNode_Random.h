// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "NarrativeNode_Random.generated.h"

USTRUCT(BlueprintType)
struct NARRATIVESYSTEM_API FRandomOutput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	FText Description;

	FRandomOutput()
		: Weight(1.f)
	{}
};

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Random : public UNarrativeNode
{
	GENERATED_BODY()

public:
	UNarrativeNode_Random();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	TArray<FRandomOutput> OutputWeights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	bool bRememberChoice = false;

	virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

	UFUNCTION(BlueprintCallable, Category = "Random")
	int32 SelectRandomOutput() const;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTooltip() const override;
#endif

private:
	UPROPERTY()
	int32 LastSelectedIndex = -1;
};
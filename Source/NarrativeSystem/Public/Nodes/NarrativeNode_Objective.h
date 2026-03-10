// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "Core/NarrativeTypes.h"
#include "NarrativeNode_Objective.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Objective : public UNarrativeNode
{
    GENERATED_BODY()

public:
    UNarrativeNode_Objective();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FName ObjectiveID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FObjectiveInfo ObjectiveInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FGameplayTag TargetTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective", meta = (ClampMin = "1"))
    int32 RequiredCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bOptional = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bHidden = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bAutoCompleteOnStart = false;

    UPROPERTY(BlueprintReadOnly, Category = "Objective")
    int32 CurrentProgress = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Objective")
    bool bIsCompleted = false;

    virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

    UFUNCTION(BlueprintCallable, Category = "Objective")
    void UpdateProgress(UNarrativeComponent* NarrativeComp, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Objective")
    void ForceComplete(UNarrativeComponent* NarrativeComp);

    UFUNCTION(BlueprintCallable, Category = "Objective")
    void ResetProgress();

    UFUNCTION(BlueprintPure, Category = "Objective")
    bool IsCompleted() const { return bIsCompleted; }

    UFUNCTION(BlueprintPure, Category = "Objective")
    float GetCompletionPercentage() const;

    UFUNCTION(BlueprintCallable, Category = "Objective")
    bool MatchesObjectiveCriteria(AActor* Actor, FName ItemID, FName LocationID) const;

#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetNodeTooltip() const override;
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

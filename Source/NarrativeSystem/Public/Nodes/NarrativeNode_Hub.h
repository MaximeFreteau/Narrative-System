// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/NarrativeNode.h"
#include "NarrativeNode_Hub.generated.h"

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeNode_Hub : public UNarrativeNode
{
    GENERATED_BODY()

public:
    UNarrativeNode_Hub();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub")
    FName HubID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub")
    FText HubDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub", meta = (MultiLine = true))
    FText HubDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub|Pins", meta = (ClampMin = "1", ClampMax = "10"))
    int32 NumInputPins = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub|Pins", meta = (ClampMin = "1", ClampMax = "10"))
    int32 NumOutputPins = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub|Options")
    bool bIsSavePoint = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub|Options")
    bool bResetStateOnEnter = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub|Options")
    bool bAllowMultipleVisits = true;

    UPROPERTY(BlueprintReadOnly, Category = "Hub")
    int32 VisitCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Hub")
    int32 LastInputPin = -1;

    virtual UNarrativeNode* Execute_Implementation(UNarrativeComponent* NarrativeComp) override;

    UFUNCTION(BlueprintCallable, Category = "Hub")
    TArray<UNarrativeNode*> GetValidOutputs(UNarrativeComponent* NarrativeComp) const;

    UFUNCTION(BlueprintCallable, Category = "Hub")
    void RegisterInputVisit(int32 InputIndex);

    UFUNCTION(BlueprintPure, Category = "Hub")
    bool AreAllInputsVisited() const;

    UFUNCTION(BlueprintCallable, Category = "Hub")
    void ResetHub();

#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetNodeTooltip() const override;
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    
    int32 GetNumInputPins() const { return NumInputPins; }
    
    int32 GetNumOutputPins() const { return NumOutputPins; }
#endif

private:
    TSet<int32> VisitedInputs;
};

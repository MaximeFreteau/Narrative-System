// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NarrativeTypes.h"
#include "NarrativeAsset.generated.h"

class UNarrativeNode;
class UEdGraph;

UCLASS(BlueprintType)
class NARRATIVESYSTEM_API UNarrativeAsset : public UObject
{
    GENERATED_BODY()

public:
    UNarrativeAsset();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
    FText NarrativeName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative", meta = (MultiLine = true))
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
    ENarrativeType Type;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
    TObjectPtr<UTexture2D> Icon;

    UPROPERTY()
    TObjectPtr<UEdGraph> NarrativeGraph;

    UPROPERTY()
    TArray<TObjectPtr<UNarrativeNode>> Nodes;

    UPROPERTY()
    TObjectPtr<UNarrativeNode> RootNode;

    UPROPERTY()
    FNarrativeMetadata Metadata;


    UFUNCTION(BlueprintCallable, Category = "Narrative")
    UNarrativeNode* GetRootNode() const { return RootNode; }

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    TArray<UNarrativeNode*> GetAllNodes() const { return Nodes; }

    UFUNCTION(BlueprintCallable, Category = "Narrative")
    UNarrativeNode* FindNodeByGuid(const FGuid& Guid) const;

    void CompileNarrativeGraph();

    void BuildMetadata();


#if WITH_EDITORONLY_DATA
    UPROPERTY()
    TObjectPtr<UEdGraph> EdGraph;
#endif

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void PostLoad() override;
#endif
};
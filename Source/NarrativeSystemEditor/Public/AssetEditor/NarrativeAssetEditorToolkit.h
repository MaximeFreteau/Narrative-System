// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Misc/NotifyHook.h"

class UNarrativeAsset;
class IDetailsView;
class SGraphEditor;
class UNarrativeEdGraph;

class FNarrativeAssetEditorToolkit : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:
    FNarrativeAssetEditorToolkit();
    virtual ~FNarrativeAssetEditorToolkit();

    void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UNarrativeAsset* InAsset);

    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;

    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

    virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
    virtual FString GetReferencerName() const override;

    virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;

private:
    TObjectPtr<UNarrativeAsset> EditingAsset;

    TSharedPtr<SGraphEditor> GraphEditor;

    TSharedPtr<IDetailsView> DetailsView;

    TSharedPtr<FUICommandList> GraphEditorCommands;

    static const FName GraphTabId;
    static const FName DetailsTabId;

    TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);

    TSharedRef<SGraphEditor> CreateGraphEditorWidget();
    TSharedRef<IDetailsView> CreateDetailsWidget();

    void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
    void OnNodeDoubleClicked(UEdGraphNode* Node);
    void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);

    void BindCommands();
    void OnCommandDelete();
    bool CanDeleteNodes() const;
    void OnCommandCopy();
    bool CanCopyNodes() const;
    void OnCommandPaste();
    bool CanPasteNodes() const;
    void OnCommandCut();
    bool CanCutNodes() const;
    void OnCommandDuplicate();
    bool CanDuplicateNodes() const;

    void CompileNarrativeAsset();

    FGraphPanelSelectionSet GetSelectedNodes() const;
};
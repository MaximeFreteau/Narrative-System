// Copyright Maxime Freteau. All Rights Reserved.

#include "AssetEditor/NarrativeAssetEditorToolkit.h"
#include "Core/NarrativeAsset.h"
#include "Core/NarrativeNode.h"
#include "Graph/NarrativeEdGraph.h"
#include "Graph/NarrativeEdGraphNode.h"
#include "Graph/NarrativeEdGraphSchema.h"
#include "GraphEditor.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Framework/Commands/GenericCommands.h"
#include "EdGraphUtilities.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ScopedTransaction.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "NarrativeAssetEditorToolkit"

const FName FNarrativeAssetEditorToolkit::GraphTabId(TEXT("NarrativeAssetEditor_Graph"));
const FName FNarrativeAssetEditorToolkit::DetailsTabId(TEXT("NarrativeAssetEditor_Details"));

FNarrativeAssetEditorToolkit::FNarrativeAssetEditorToolkit()
    : EditingAsset(nullptr)
{
}

FNarrativeAssetEditorToolkit::~FNarrativeAssetEditorToolkit()
{
}

void FNarrativeAssetEditorToolkit::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UNarrativeAsset* InAsset)
{
    EditingAsset = InAsset;

    if (!EditingAsset->NarrativeGraph)
    {
        EditingAsset->NarrativeGraph = NewObject<UNarrativeEdGraph>(EditingAsset, UNarrativeEdGraph::StaticClass(), NAME_None, RF_Transactional);
        if (UNarrativeEdGraph* NarrativeEdGraph = Cast<UNarrativeEdGraph>(EditingAsset->NarrativeGraph))
        {
            NarrativeEdGraph->NarrativeAsset = EditingAsset;
        }

#if WITH_EDITORONLY_DATA
        EditingAsset->EdGraph = EditingAsset->NarrativeGraph;
#endif

        EditingAsset->NarrativeGraph->Schema = UNarrativeEdGraphSchema::StaticClass();
        
        const UEdGraphSchema* Schema = EditingAsset->NarrativeGraph->GetSchema();
        Schema->CreateDefaultNodesForGraph(*EditingAsset->NarrativeGraph);
    }

    BindCommands();

    GraphEditor = CreateGraphEditorWidget();
    DetailsView = CreateDetailsWidget();

    const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_NarrativeAssetEditor_Layout_v1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
            ->Split
            (
                FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.7f)
                    ->AddTab(GraphTabId, ETabState::OpenedTab)
                )
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.3f)
                    ->AddTab(DetailsTabId, ETabState::OpenedTab)
                )
            )
        );

    const bool bCreateDefaultStandaloneMenu = true;
    const bool bCreateDefaultToolbar = true;

    FAssetEditorToolkit::InitAssetEditor(
        Mode,
        InitToolkitHost,
        FName("NarrativeAssetEditorApp"),
        StandaloneDefaultLayout,
        bCreateDefaultStandaloneMenu,
        bCreateDefaultToolbar,
        EditingAsset
    );
}

FName FNarrativeAssetEditorToolkit::GetToolkitFName() const
{
    return FName("NarrativeAssetEditor");
}

FText FNarrativeAssetEditorToolkit::GetBaseToolkitName() const
{
    return LOCTEXT("AppLabel", "Narrative Asset Editor");
}

FString FNarrativeAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
    return LOCTEXT("WorldCentricTabPrefix", "Narrative Asset ").ToString();
}

FLinearColor FNarrativeAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.5f, 0.8f, 0.5f, 0.5f);
}

void FNarrativeAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_NarrativeAssetEditor", "Narrative Asset Editor"));
    auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    InTabManager->RegisterTabSpawner(GraphTabId, FOnSpawnTab::CreateSP(this, &FNarrativeAssetEditorToolkit::SpawnTab_Graph))
        .SetDisplayName(LOCTEXT("GraphTab", "Graph"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

    InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FNarrativeAssetEditorToolkit::SpawnTab_Details))
        .SetDisplayName(LOCTEXT("DetailsTab", "Details"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FNarrativeAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

    InTabManager->UnregisterTabSpawner(GraphTabId);
    InTabManager->UnregisterTabSpawner(DetailsTabId);
}

void FNarrativeAssetEditorToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
    Collector.AddReferencedObject(EditingAsset);
}

FString FNarrativeAssetEditorToolkit::GetReferencerName() const
{
    return TEXT("FNarrativeAssetEditorToolkit");
}

void FNarrativeAssetEditorToolkit::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
    if (PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
    {
        if (GraphEditor.IsValid())
        {
            GraphEditor->NotifyGraphChanged();
        }

        CompileNarrativeAsset();
    }
}

TSharedRef<SDockTab> FNarrativeAssetEditorToolkit::SpawnTab_Graph(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == GraphTabId);

    return SNew(SDockTab)
        .Label(LOCTEXT("GraphTab", "Graph"))
        [
            GraphEditor.ToSharedRef()
        ];
}

TSharedRef<SDockTab> FNarrativeAssetEditorToolkit::SpawnTab_Details(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == DetailsTabId);

    return SNew(SDockTab)
        .Label(LOCTEXT("DetailsTab", "Details"))
        [
            DetailsView.ToSharedRef()
        ];
}

TSharedRef<SGraphEditor> FNarrativeAssetEditorToolkit::CreateGraphEditorWidget()
{
    FGraphAppearanceInfo AppearanceInfo;
    AppearanceInfo.CornerText = LOCTEXT("CornerText", "NARRATIVE");

    SGraphEditor::FGraphEditorEvents InEvents;
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FNarrativeAssetEditorToolkit::OnSelectedNodesChanged);
    InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FNarrativeAssetEditorToolkit::OnNodeDoubleClicked);
    InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FNarrativeAssetEditorToolkit::OnNodeTitleCommitted);

    return SNew(SGraphEditor)
        .AdditionalCommands(GraphEditorCommands)
        .IsEditable(true)
        .Appearance(AppearanceInfo)
        .GraphToEdit(EditingAsset->NarrativeGraph)
        .GraphEvents(InEvents)
        .ShowGraphStateOverlay(false);
}

TSharedRef<IDetailsView> FNarrativeAssetEditorToolkit::CreateDetailsWidget()
{
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsViewArgs.NotifyHook = this;

    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    TSharedRef<IDetailsView> DetailsViewWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

    DetailsViewWidget->SetObject(EditingAsset);

    return DetailsViewWidget;
}

void FNarrativeAssetEditorToolkit::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{

    TArray<UObject*> Selection;

    if (NewSelection.Num() > 0)
    {
        for (UObject* SelectedObject : NewSelection)
        {
            UE_LOG(LogTemp, Warning, TEXT("[NarrativeEditor] Selected: %s"), *GetNameSafe(SelectedObject));

            if (UNarrativeEdGraphNode* GraphNode = Cast<UNarrativeEdGraphNode>(SelectedObject))
            {
                UE_LOG(LogTemp, Warning, TEXT("[NarrativeEditor] Is NarrativeEdGraphNode"));

                if (!GraphNode->RuntimeNode)
                {
                    UE_LOG(LogTemp, Warning, TEXT("[NarrativeEditor] Creating RuntimeNode..."));
                    GraphNode->CreateRuntimeNode();
                }

                if (UNarrativeNode* RuntimeNode = GraphNode->RuntimeNode.Get())
                {
                    if (IsValid(RuntimeNode))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("[NarrativeEditor] RuntimeNode is valid: %s"), *GetNameSafe(RuntimeNode));
                        Selection.Add(RuntimeNode);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("[NarrativeEditor] RuntimeNode is INVALID!"));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("[NarrativeEditor] RuntimeNode is NULL!"));
                }
            }
        }
    }

    if (Selection.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[NarrativeEditor] No valid selection, showing asset"));
        if (EditingAsset.Get() && IsValid(EditingAsset.Get()))
        {
            Selection.Add(EditingAsset.Get());
        }
    }

    if (Selection.Num() > 0 && DetailsView.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[NarrativeEditor] Setting %d objects in DetailsView"), Selection.Num());
        DetailsView->SetObjects(Selection);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[NarrativeEditor] Cannot set objects in DetailsView!"));
    }
}

void FNarrativeAssetEditorToolkit::OnNodeDoubleClicked(UEdGraphNode* Node)
{
}

void FNarrativeAssetEditorToolkit::OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged)
{
    if (NodeBeingChanged)
    {
        const FScopedTransaction Transaction(LOCTEXT("RenameNode", "Rename Node"));
        NodeBeingChanged->Modify();
        NodeBeingChanged->OnRenameNode(NewText.ToString());
    }
}

void FNarrativeAssetEditorToolkit::BindCommands()
{
    GraphEditorCommands = MakeShareable(new FUICommandList);

    GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
        FExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::OnCommandDelete),
        FCanExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::CanDeleteNodes)
    );

    GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
        FExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::OnCommandCopy),
        FCanExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::CanCopyNodes)
    );

    GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
        FExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::OnCommandPaste),
        FCanExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::CanPasteNodes)
    );

    GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
        FExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::OnCommandCut),
        FCanExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::CanCutNodes)
    );

    GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
        FExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::OnCommandDuplicate),
        FCanExecuteAction::CreateSP(this, &FNarrativeAssetEditorToolkit::CanDuplicateNodes)
    );
}

void FNarrativeAssetEditorToolkit::OnCommandDelete()
{
    const FScopedTransaction Transaction(LOCTEXT("DeleteNodes", "Delete Nodes"));

    const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
    GraphEditor->GetCurrentGraph()->Modify();

    for (UObject* Object : SelectedNodes)
    {
        if (UEdGraphNode* Node = Cast<UEdGraphNode>(Object))
        {
            if (Node->CanUserDeleteNode())
            {
                Node->Modify();
                Node->DestroyNode();
            }
        }
    }

    GraphEditor->ClearSelectionSet();
    GraphEditor->NotifyGraphChanged();
    CompileNarrativeAsset();
}

bool FNarrativeAssetEditorToolkit::CanDeleteNodes() const
{
    const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
    
    for (UObject* Object : SelectedNodes)
    {
        if (UEdGraphNode* Node = Cast<UEdGraphNode>(Object))
        {
            if (Node->CanUserDeleteNode())
            {
                return true;
            }
        }
    }

    return false;
}

void FNarrativeAssetEditorToolkit::OnCommandCopy()
{
    const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

    FString ExportedText;
    FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
    FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FNarrativeAssetEditorToolkit::CanCopyNodes() const
{
    const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
    return SelectedNodes.Num() > 0;
}

void FNarrativeAssetEditorToolkit::OnCommandPaste()
{
    const FScopedTransaction Transaction(LOCTEXT("PasteNodes", "Paste Nodes"));

    GraphEditor->ClearSelectionSet();
    GraphEditor->GetCurrentGraph()->Modify();

    const FVector2D PasteLocation = GraphEditor->GetPasteLocation();

    FString TextToImport;
    FPlatformApplicationMisc::ClipboardPaste(TextToImport);

    TSet<UEdGraphNode*> PastedNodes;
    FEdGraphUtilities::ImportNodesFromText(GraphEditor->GetCurrentGraph(), TextToImport, PastedNodes);

    for (UEdGraphNode* Node : PastedNodes)
    {
        Node->NodePosX = (int32)(PasteLocation.X);
        Node->NodePosY = (int32)(PasteLocation.Y);
        Node->SnapToGrid(16);

        GraphEditor->SetNodeSelection(Node, true);
    }

    GraphEditor->NotifyGraphChanged();
    CompileNarrativeAsset();
}

bool FNarrativeAssetEditorToolkit::CanPasteNodes() const
{
    FString ClipboardContent;
    FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

    return FEdGraphUtilities::CanImportNodesFromText(GraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FNarrativeAssetEditorToolkit::OnCommandCut()
{
    OnCommandCopy();
    OnCommandDelete();
}

bool FNarrativeAssetEditorToolkit::CanCutNodes() const
{
    return CanCopyNodes() && CanDeleteNodes();
}

void FNarrativeAssetEditorToolkit::OnCommandDuplicate()
{
    OnCommandCopy();
    OnCommandPaste();
}

bool FNarrativeAssetEditorToolkit::CanDuplicateNodes() const
{
    return CanCopyNodes();
}

FGraphPanelSelectionSet FNarrativeAssetEditorToolkit::GetSelectedNodes() const
{
    return GraphEditor->GetSelectedNodes();
}

void FNarrativeAssetEditorToolkit::CompileNarrativeAsset()
{
    if (EditingAsset && EditingAsset->NarrativeGraph)
    {
        UNarrativeEdGraph* EdGraph = Cast<UNarrativeEdGraph>(EditingAsset->NarrativeGraph);
        if (EdGraph)
        {
            EdGraph->CompileGraph();
        }
    }
}

#undef LOCTEXT_NAMESPACE
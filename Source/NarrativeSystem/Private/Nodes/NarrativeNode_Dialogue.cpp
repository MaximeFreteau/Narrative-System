// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Dialogue.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Dialogue::UNarrativeNode_Dialogue()
{
    NodeTitle = FText::FromString("Dialogue");
    
    FDialogueLine DefaultLine;
    DefaultLine.SpeakerID = FName("Speaker");
    DefaultLine.SpeakerName = FText::FromString("Speaker");
    DefaultLine.DialogueText = FText::FromString("Enter dialogue text here...");
    Lines.Add(DefaultLine);
}

UNarrativeNode* UNarrativeNode_Dialogue::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return nullptr;
    }

    DisplayDialogue(NarrativeComp);

    return this;
}

bool UNarrativeNode_Dialogue::CanExecute_Implementation(UNarrativeComponent* NarrativeComp) const
{
    return true;
}

void UNarrativeNode_Dialogue::DisplayDialogue(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return;
    }

    NarrativeComp->OnDialogueStarted.Broadcast(Lines, Choices);

    for (const FDialogueLine& Line : Lines)
    {
        NarrativeComp->OnDialogueLineDisplayed.Broadcast(
            Line.SpeakerID,
            Line.SpeakerName,
            Line.DialogueText,
            Line.Portrait
        );
    }

    if (Choices.Num() > 0)
    {
        DisplayChoices(NarrativeComp);
    }

    NarrativeComp->bIsInDialogue = true;
    NarrativeComp->TotalDialoguesSeenCount++;
}

void UNarrativeNode_Dialogue::DisplayChoices(UNarrativeComponent* NarrativeComp)
{
    if (!NarrativeComp)
    {
        return;
    }

    TArray<FDialogueChoice> ValidChoices;
    for (const FDialogueChoice& Choice : Choices)
    {
        if (!Choice.bHidden)
        {
            ValidChoices.Add(Choice);
        }
    }

    NarrativeComp->OnChoicesPresented.Broadcast(ValidChoices);
}

#if WITH_EDITOR
FText UNarrativeNode_Dialogue::GetNodeTitle() const
{
    if (!NodeTitle.IsEmpty())
    {
        return NodeTitle;
    }

    if (Lines.Num() > 0 && !Lines[0].DialogueText.IsEmpty())
    {
        FString Preview = Lines[0].DialogueText.ToString();
        if (Preview.Len() > 40)
        {
            Preview = Preview.Left(37) + TEXT("...");
        }
        return FText::FromString(Preview);
    }

    return FText::FromString("Dialogue");
}

FLinearColor UNarrativeNode_Dialogue::GetNodeTitleColor() const
{
    return FLinearColor(0.2f, 0.6f, 1.0f);
}

FText UNarrativeNode_Dialogue::GetNodeTooltip() const
{
    FString TooltipText = TEXT("Dialogue Node\n\n");
    
    if (Lines.Num() > 0)
    {
        TooltipText += FString::Printf(TEXT("Lines: %d\n"), Lines.Num());
    }
    
    if (Choices.Num() > 0)
    {
        TooltipText += FString::Printf(TEXT("Choices: %d\n"), Choices.Num());
    }

    TooltipText += FString::Printf(TEXT("Mode: %s"), 
        ContinueMode == EDialogueContinueMode::Manual ? TEXT("Manual") :
        ContinueMode == EDialogueContinueMode::Auto ? TEXT("Auto") : TEXT("Wait Audio"));

    return FText::FromString(TooltipText);
}
#endif
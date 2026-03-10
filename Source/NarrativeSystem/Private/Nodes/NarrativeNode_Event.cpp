// Copyright Maxime Freteau. All Rights Reserved.

#include "Nodes/NarrativeNode_Event.h"
#include "Core/NarrativeComponent.h"

UNarrativeNode_Event::UNarrativeNode_Event()
{
	NodeTitle = FText::FromString("Event");
}

UNarrativeNode* UNarrativeNode_Event::Execute_Implementation(UNarrativeComponent* NarrativeComp)
{
	if (!NarrativeComp)
	{
		return nullptr;
	}

	for (const auto& Pair : IntParameters)
	{
		UE_LOG(LogTemp, Log, TEXT("  - %s: %d"), *Pair.Key.ToString(), Pair.Value);
	}

	return GetDefaultNextNode();
}

#if WITH_EDITOR
FText UNarrativeNode_Event::GetNodeTitle() const
{
	if (EventTag.IsValid())
	{
		return FText::Format(FText::FromString("Event: {0}"), FText::FromString(EventTag.ToString()));
	}
	return FText::FromString("Event");
}

FLinearColor UNarrativeNode_Event::GetNodeTitleColor() const
{
	return FLinearColor(0.8f, 0.2f, 0.8f);
}

FText UNarrativeNode_Event::GetNodeTooltip() const
{
	FString TooltipText = TEXT("Event Node\n\n");
    
	if (EventTag.IsValid())
	{
		TooltipText += FString::Printf(TEXT("Tag: %s\n"), *EventTag.ToString());
	}
    
	if (IntParameters.Num() > 0 || StringParameters.Num() > 0 || BoolParameters.Num() > 0)
	{
		TooltipText += FString::Printf(TEXT("\nParameters: %d"), 
			IntParameters.Num() + StringParameters.Num() + BoolParameters.Num());
	}

	return FText::FromString(TooltipText);
}
#endif
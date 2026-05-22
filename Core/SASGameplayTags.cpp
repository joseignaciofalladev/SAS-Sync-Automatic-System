#include "SASGameplayTags.h"

#include "GameplayTagsManager.h"

FSASGameplayTags FSASGameplayTags::GameplayTags;

void FSASGameplayTags::InitializeNativeTags()
{
	/////////////////////////////////////
	// INTERACTION
	/////////////////////////////////////

	GameplayTags.Interaction =
		AddNativeTag(
			TEXT("SAS.Interaction"),
			TEXT("Base interaction tag.")
		);

	GameplayTags.InteractionAvailable =
		AddNativeTag(
			TEXT("SAS.Interaction.Available"),
			TEXT("Interaction is available.")
		);

	GameplayTags.InteractionBlocked =
		AddNativeTag(
			TEXT("SAS.Interaction.Blocked"),
			TEXT("Interaction is blocked.")
		);

	GameplayTags.InteractionBusy =
		AddNativeTag(
			TEXT("SAS.Interaction.Busy"),
			TEXT("Interaction is busy.")
		);

	GameplayTags.InteractionDisabled =
		AddNativeTag(
			TEXT("SAS.Interaction.Disabled"),
			TEXT("Interaction disabled.")
		);

	/////////////////////////////////////
	// STATE
	/////////////////////////////////////

	GameplayTags.StateFocused =
		AddNativeTag(
			TEXT("SAS.State.Focused"),
			TEXT("Actor currently focused.")
		);

	GameplayTags.StateInteracting =
		AddNativeTag(
			TEXT("SAS.State.Interacting"),
			TEXT("Actor currently interacting.")
		);

	/////////////////////////////////////
	// INPUT
	/////////////////////////////////////

	GameplayTags.InputPress =
		AddNativeTag(
			TEXT("SAS.Input.Press"),
			TEXT("Press interaction.")
		);

	GameplayTags.InputHold =
		AddNativeTag(
			TEXT("SAS.Input.Hold"),
			TEXT("Hold interaction.")
		);

	GameplayTags.InputToggle =
		AddNativeTag(
			TEXT("SAS.Input.Toggle"),
			TEXT("Toggle interaction.")
		);

	GameplayTags.InputAutomatic =
		AddNativeTag(
			TEXT("SAS.Input.Automatic"),
			TEXT("Automatic interaction.")
		);

	/////////////////////////////////////
	// CONTEXT
	/////////////////////////////////////

	GameplayTags.ContextUsable =
		AddNativeTag(
			TEXT("SAS.Context.Usable"),
			TEXT("Object usable.")
		);

	GameplayTags.ContextLocked =
		AddNativeTag(
			TEXT("SAS.Context.Locked"),
			TEXT("Object locked.")
		);

	GameplayTags.ContextRequiresItem =
		AddNativeTag(
			TEXT("SAS.Context.RequiresItem"),
			TEXT("Requires specific item.")
		);

	GameplayTags.ContextRestricted =
		AddNativeTag(
			TEXT("SAS.Context.Restricted"),
			TEXT("Restricted interaction.")
		);

	/////////////////////////////////////
	// NETWORK
	/////////////////////////////////////

	GameplayTags.NetworkPredicted =
		AddNativeTag(
			TEXT("SAS.Network.Predicted"),
			TEXT("Client predicted interaction.")
		);

	GameplayTags.NetworkAuthority =
		AddNativeTag(
			TEXT("SAS.Network.Authority"),
			TEXT("Server authority interaction.")
		);
}

FGameplayTag FSASGameplayTags::AddNativeTag(
	const FString& TagName,
	const FString& TagComment)
{
	return UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(*TagName),
		TagComment
	);
}
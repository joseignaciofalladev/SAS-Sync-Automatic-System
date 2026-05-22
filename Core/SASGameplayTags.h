#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Centralized Gameplay Tag registry for SAS.
 * This class owns and initializes every native gameplay tag used
 * internally by the framework.
 * Compatible with Unreal Engine 4.26+
 */
class SAS_API FSASGameplayTags
{
public:
	// Initializes all native SAS gameplay tags.
	// Must be called during module startup.
	static void InitializeNativeTags();

	// Returns singleton instance
	static const FSASGameplayTags& Get()
	{return GameplayTags;}

public:
	/////////////////////////////////////
	// INTERACTION
	/////////////////////////////////////

	FGameplayTag Interaction;
	FGameplayTag InteractionAvailable;
	FGameplayTag InteractionBlocked;
	FGameplayTag InteractionBusy;
	FGameplayTag InteractionDisabled;

	/////////////////////////////////////
	// STATE
	/////////////////////////////////////

	FGameplayTag StateFocused;
	FGameplayTag StateInteracting;

	/////////////////////////////////////
	// INPUT
	/////////////////////////////////////

	FGameplayTag InputPress;
	FGameplayTag InputHold;
	FGameplayTag InputToggle;
	FGameplayTag InputAutomatic;

	/////////////////////////////////////
	// CONTEXT
	/////////////////////////////////////

	FGameplayTag ContextUsable;
	FGameplayTag ContextLocked;
	FGameplayTag ContextRequiresItem;
	FGameplayTag ContextRestricted;

	/////////////////////////////////////
	// NETWORK
	/////////////////////////////////////

	FGameplayTag NetworkPredicted;
	FGameplayTag NetworkAuthority;

private:
	static FSASGameplayTags GameplayTags;
	static FGameplayTag AddNativeTag(
		const FString& TagName,
		const FString& TagComment
	);
};
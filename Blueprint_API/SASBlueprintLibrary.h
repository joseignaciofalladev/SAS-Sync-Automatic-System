#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SASBlueprintLibrary.generated.h"

class UInteractionComponent;
class UInteractionProgressComponent;

/**
 * Blueprint utility helpers for SAS.
 *
 * Provides convenient access to:
 * - interaction components
 * - focused targets
 * - timed interactions
 * - interface validation
 */
UCLASS()
class SAS_API USASBlueprintLibrary final
	: public UBlueprintFunctionLibrary{
	GENERATED_BODY()
public:

	/////////////////////////////////////
    // INTERACTION COMPONENT
	/////////////////////////////////////

	// Gets interaction component from actor
	UFUNCTION(
		BlueprintPure,
		Category="SAS|Interaction",
		meta=(DefaultToSelf="Actor")
	)
	static UInteractionComponent*GetInteractionComponent(AActor* Actor);

	// Gets progress component from actor
	UFUNCTION(
		BlueprintPure,
		Category="SAS|Interaction",
		meta=(DefaultToSelf="Actor")
	)
	static UInteractionProgressComponent*GetInteractionProgressComponent(AActor* Actor);

	/////////////////////////////////////
	// FOCUSED TARGET
	/////////////////////////////////////

	// Gets currently focused actor
	UFUNCTION(
		BlueprintPure,
		Category="SAS|Interaction",
		meta=(DefaultToSelf="Actor")
	)
	static AActor*GetFocusedInteractionActor(AActor* Actor);

	/////////////////////////////////////
	// INTERACTION
	/////////////////////////////////////

	// Attempts interaction
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|Interaction",
		meta=(DefaultToSelf="Actor")
	)
	static bool Interact(AActor* Actor);

	// Starts timed interaction
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|Interaction",
		meta=(DefaultToSelf="Actor")
	)
	static bool StartTimedInteraction(AActor* Actor, float Duration);

	// Cancels timed interaction
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|Interaction",
		meta=(DefaultToSelf="Actor")
	)
	static void CancelTimedInteraction(AActor* Actor);

	/////////////////////////////////////
	// VALIDATION
	/////////////////////////////////////

	// Returns true if actor
	// Implements interactor interface
	UFUNCTION(
		BlueprintPure,
		Category="SAS|Validation"
	)
	static bool IsInteractable(AActor* Actor);

	UFUNCTION(
		BlueprintPure,
		Category="SAS|Validation"
	)
	static bool IsInteractor(AActor* Actor);
};

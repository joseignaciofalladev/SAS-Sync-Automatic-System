#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SASTypes.h"
#include "Interfaces/InteractableInterface.h"
#include "InteractableComponent.generated.h"

/////////////////////////////////////
// DELEGATES
/////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FSASInteractableActorSignature,
	AActor*,
	Interactor
);

/**
 * Reusable interactable implementation for SAS.
 *
 * Plug-and-play interaction component that can be
 * attached to any actor to make it interactable.
 *
 * Features:
 * - Priority system
 * - State management
 * - Gameplay tags
 * - Timed interactions
 * - Focus lifecycle
 * - Blueprint events
*/

UCLASS(
	ClassGroup=(SAS),
	BlueprintType,
	Blueprintable,
	meta=(BlueprintSpawnableComponent)
)
class SAS_API UInteractableComponent final
	: public UActorComponent,
	  public IInteractableInterface
{
	GENERATED_BODY()

public:

	UInteractableComponent();

protected:

	/////////////////////////////////////
	// UACTOR COMPONENT
	/////////////////////////////////////

	virtual void BeginPlay() override;

	/////////////////////////////////////
	// INTERACTABLE INTERFACE
	/////////////////////////////////////

public:

	virtual bool CanInteract_Implementation(
		AActor* Interactor
	) const override;

	virtual float GetInteractionPriority_Implementation()
		const override;

	virtual FGameplayTagContainer
	GetInteractionTags_Implementation()
		const override;

	virtual float GetInteractionScore_Implementation(
		const FSASInteractionContext& Context
	) const override;

	virtual void OnFocusStart_Implementation(
		AActor* Interactor
	) override;

	virtual void OnFocusEnd_Implementation(
		AActor* Interactor
	) override;

	virtual void OnInteractionStarted_Implementation(
		AActor* Interactor
	) override;

	virtual void OnInteractionCompleted_Implementation(
		AActor* Interactor
	) override;

	virtual void OnInteractionCancelled_Implementation(
		AActor* Interactor
	) override;

	virtual ESASInteractionState
	GetInteractionState_Implementation()
		const override;

	/////////////////////////////////////
	// CONFIGURATION
	/////////////////////////////////////

public:

    // Whether this object can be interacted with
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Interaction"
	)
	bool bEnabled;

	// Interaction priority
	// Higher value = preferred target
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Interaction",
		meta=(
			ClampMin="0.0",
			UIMin="0.0",
			UIMax="100.0"
		)
	)
	float InteractionPriority;

    // Interaction mode
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Interaction"
	)
	ESASInteractionMode InteractionMode;

	// Interaction input type
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Interaction"
	)
	ESASInteractionInputType InputType;

	// Timed interaction duration
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Interaction",
		meta=(
			EditCondition="InteractionMode == ESASInteractionMode::Timed",
			ClampMin="0.1",
			UIMin="0.1",
			UIMax="10.0"
		)
	)
	float InteractionDuration;

	// Gameplay tags for filtering/context
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Interaction"
	)
	FGameplayTagContainer InteractionTags;

	/////////////////////////////////////
	// STATE
	/////////////////////////////////////

protected:

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category="SAS|State"
	)
	ESASInteractionState InteractionState;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category="SAS|State"
	)
	TWeakObjectPtr<AActor> CurrentInteractor;

	/////////////////////////////////////
	// EVENTS
    /////////////////////////////////////

public:

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASInteractableActorSignature
	OnFocusStarted;

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASInteractableActorSignature
	OnFocusEnded;

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASInteractableActorSignature
	OnInteractionStartedEvent;

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASInteractableActorSignature
	OnInteractionCompletedEvent;

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASInteractableActorSignature
	OnInteractionCancelledEvent;

	/////////////////////////////////////
	// HELPERS
	/////////////////////////////////////

public:

	UFUNCTION(
		BlueprintCallable,
		Category="SAS|Interaction"
	)
	void SetInteractionEnabled(bool bNewEnabled);

	UFUNCTION(
		BlueprintPure,
		Category="SAS|Interaction"
	)
	FORCEINLINE bool IsInteractionEnabled() const {
        return bEnabled;
	}

	UFUNCTION(
		BlueprintPure,
		Category="SAS|Interaction"
	)
	FORCEINLINE AActor* GetCurrentInteractor() const {
        return CurrentInteractor.Get();
	}

	UFUNCTION(
		BlueprintPure,
		Category="SAS|Interaction"
	)
	FORCEINLINE bool IsFocused() const {
		return InteractionState ==ESASInteractionState::Focused;
	}

	UFUNCTION(
		BlueprintPure,
		Category="SAS|Interaction"
	)
    FORCEINLINE bool IsInteracting() const {
		return InteractionState ==ESASInteractionState::Interacting;
	}
};

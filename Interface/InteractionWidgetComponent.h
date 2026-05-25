#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionWidgetComponent.generated.h"

class UInteractionComponent;
class UInteractionProgressComponent;
class USASInteractionWidget;

/////////////////////////////////////
// DELEGATES
/////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FSASWidgetTargetSignature,
	AActor*,
	TargetActor
);

/**
 * UI bridge between SAS interaction runtime
 * and interaction widgets.
 *
 * Responsibilities:
 * - Focus listening
 * - Widget visibility
 * - Widget target updates
 * - Progress synchronization
 * - Prompt lifecycle
 *
 * This component intentionally contains
 * no gameplay logic.
 */
UCLASS(
	ClassGroup=(SAS),
	BlueprintType,
	Blueprintable,
	meta=(BlueprintSpawnableComponent)
)
class SAS_API UInteractionWidgetComponent final
	: public UActorComponent{
	GENERATED_BODY()
public:
	UInteractionWidgetComponent();
protected:

	/////////////////////////////////////
	// UACTOR COMPONENT
	/////////////////////////////////////

	virtual void BeginPlay() override;

	virtual void EndPlay(
		const EEndPlayReason::Type
		EndPlayReason
	) override;

	/////////////////////////////////////
	// INITIALIZATION
	/////////////////////////////////////

	// Finds owner interaction component
	void InitializeInteractionComponent();

	// Finds owner progress component
	void InitializeProgressComponent();

	// Creates interaction widget
	void CreateInteractionWidget();

	/////////////////////////////////////
	// FOCUS EVENTS
	/////////////////////////////////////

	UFUNCTION()
	void HandleFocusedActorChanged(
		AActor* PreviousTarget,
		AActor* NewTarget
	);

	/////////////////////////////////////
	// PROGRESS EVENTS
	/////////////////////////////////////

	UFUNCTION()
	void HandleProgressUpdated(
		AActor* TargetActor,
		float Progress
	);

	UFUNCTION()
	void HandleProgressCompleted(AActor* TargetActor);

	UFUNCTION()
	void HandleProgressCancelled(AActor* TargetActor);

	/////////////////////////////////////
	// WIDGET CONTROL
	/////////////////////////////////////
public:

	// Shows interaction widget
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|UI"
	)
	void ShowWidget(AActor* TargetActor);

	// Hides interaction widget
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|UI"
	)
	void HideWidget();

	// Updates target actor
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|UI"
	)
	void SetTargetActor(AActor* NewTarget);

	/////////////////////////////////////
	// GETTERS
	/////////////////////////////////////
public:

	UFUNCTION(
		BlueprintPure,
		Category="SAS|UI"
	)
	FORCEINLINE bool
	IsWidgetVisible() const{
		return bWidgetVisible;
	}

	UFUNCTION(
		BlueprintPure,
		Category="SAS|UI"
	)
	FORCEINLINE AActor*
	GetCurrentTarget() const{
		return CurrentTarget.Get();
	}

	UFUNCTION(
		BlueprintPure,
		Category="SAS|UI"
	)
	FORCEINLINE USASInteractionWidget*
	GetInteractionWidget() const{
		return InteractionWidget;
	}

	/////////////////////////////////////
	// CONFIGURATION
	/////////////////////////////////////
public:

    // Widget class to spawn
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|UI"
	)
	TSubclassOf<USASInteractionWidget>InteractionWidgetClass;

    // Automatically show widget on focus
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|UI"
	)
	bool bAutoShowOnFocus;

	// Automatically hide widget
	// when focus is lost
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|UI"
	)
	bool bAutoHideOnFocusLost;

	// Destroys widget instance
	// when hidden
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|UI"
	)
	bool bDestroyWidgetOnHide;

	/////////////////////////////////////
    // STATE
	/////////////////////////////////////

protected:
	// Cached interaction component
	UPROPERTY(Transient)
	UInteractionComponent*InteractionComponent;

	// Cached progress component
	UPROPERTY(Transient)
	UInteractionProgressComponent*InteractionProgressComponent;

	// Runtime widget instance
	UPROPERTY(Transient)
	USASInteractionWidget*InteractionWidget;

	// Current focused actor
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor>CurrentTarget;

	// Visibility state
	UPROPERTY(Transient)
	bool bWidgetVisible;

	/////////////////////////////////////
	// EVENTS
	/////////////////////////////////////

public:
	// Fired when widget shown
	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASWidgetTargetSignature OnWidgetShown;

	// Fired when widget hidden
	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASWidgetTargetSignature OnWidgetHidden;

	// Fired when target changes
	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASWidgetTargetSignature OnWidgetTargetChanged;
};

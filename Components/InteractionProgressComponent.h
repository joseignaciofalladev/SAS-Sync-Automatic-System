#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionProgressComponent.generated.h"

class UInteractionComponent;

/////////////////////////////////////
// DELEGATES
/////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FSASInteractionProgressSignature,
	AActor*,
	TargetActor,
	float,
	Progress
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FSASProgressTargetSignature,
	AActor*,
	TargetActor
);

/**
 * Runtime timed interaction system.
 *
 * Handles:
 * - Hold interactions
 * - Timed interactions
 * - Progress updates
 * - Cancellation
 * - Completion
 *
 * Decoupled from interactables.
 */
UCLASS(
	ClassGroup=(SAS),
	BlueprintType,
	Blueprintable,
	meta=(BlueprintSpawnableComponent)
)
class SAS_API UInteractionProgressComponent final
	: public UActorComponent
{
	GENERATED_BODY()
public:
	UInteractionProgressComponent();
protected:
	
	/////////////////////////////////////
	// UACTOR COMPONENT
	/////////////////////////////////////
	virtual void BeginPlay() override;

	virtual void EndPlay(
		const EEndPlayReason::Type
		EndPlayReason
	) override;

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction*
		ThisTickFunction
	) override;

	/////////////////////////////////////
	//INITIALIZATION
	/////////////////////////////////////

	void InitializeInteractionComponent();

	/////////////////////////////////////
	// PROGRESS
	/////////////////////////////////////
public:
	
	// Starts timed interaction
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|Progress"
	)
	bool StartProgress(
		AActor* TargetActor,
		float Duration
	);

	// Cancels current progress
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|Progress"
	)
	void CancelProgress();

	// Immediately completes progress
	UFUNCTION(
		BlueprintCallable,
		Category="SAS|Progress"
	)
	void CompleteProgress();

	/////////////////////////////////////
	// GETTERS
	/////////////////////////////////////

public:
	UFUNCTION(
		BlueprintPure,
		Category="SAS|Progress"
	)
	FORCEINLINE bool
	IsProgressActive() const{
		return bProgressActive;
	}

	UFUNCTION(
		BlueprintPure,
		Category="SAS|Progress"
	)
	FORCEINLINE float
	GetProgress() const{
		return Progress;
	}

	UFUNCTION(
		BlueprintPure,
		Category="SAS|Progress"
	)
	FORCEINLINE AActor*
	GetTargetActor() const{
		return TargetActor.Get();
	}

	/////////////////////////////////////
	// CONFIGURATION
	/////////////////////////////////////
public:

	// Automatically cancel when focus lost
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Progress"
	)
	bool bCancelOnFocusLost;

	// Auto-complete interaction
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category="SAS|Progress"
	)
	bool bAutoCompleteInteraction;

	/////////////////////////////////////
	// STATE
	/////////////////////////////////////
protected:
	UPROPERTY(Transient)
	UInteractionComponent*
	InteractionComponent;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category="SAS|State"
	)
	TWeakObjectPtr<AActor>
	TargetActor;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category="SAS|State"
	)
	bool bProgressActive;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category="SAS|State"
	)
	float Progress;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category="SAS|State"
	)
	float CurrentTime;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category="SAS|State"
	)
	float Duration;

	/////////////////////////////////////
	// EVENTS
	/////////////////////////////////////
public:
	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASInteractionProgressSignature OnProgressUpdated;

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASProgressTargetSignature OnProgressStarted;

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASProgressTargetSignature OnProgressCompleted;

	UPROPERTY(
		BlueprintAssignable,
		Category="SAS|Events"
	)
	FSASProgressTargetSignatureOnProgressCancelled;
};

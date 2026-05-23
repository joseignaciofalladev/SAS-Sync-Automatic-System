#include "Components/InteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Interfaces/InteractorInterface.h"
#include "Interfaces/InteractableInterface.h"
#include "SASLog.h"
#include "SASSettings.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InteractionState = ESASInteractionState::Inactive;

	DetectionAccumulator = 0.f;
	OverrideInteractionDistance = -1.f;
	OverrideDetectionInterval = -1.f;
	bEnableDebugDraw = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	SAS_LOG(
		TEXT("Interaction Component initialized on %s"),
		*GetNameSafe(GetOwner())
	);
}

void UInteractionComponent::EndPlay(
	const EEndPlayReason::Type EndPlayReason) {
	ClearFocus();
	Super::EndPlay(EndPlayReason);
}

void UInteractionComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(
		DeltaTime,
		TickType,
		ThisTickFunction
	);

	const USASSettings* Settings = USASSettings::Get();

	if (!Settings) {return;}

	const float DetectionInterval =
		OverrideDetectionInterval > 0.f
		? OverrideDetectionInterval
		: Settings->DetectionInterval;

	DetectionAccumulator += DeltaTime;

	if (DetectionAccumulator >= DetectionInterval) {
		UpdateFocus();
		DetectionAccumulator = 0.f;
	}
}

void UInteractionComponent::UpdateFocus() {
	AActor* BestCandidate = FindBestInteractionCandidate();
	if (BestCandidate != FocusedActor.Get()) {SetFocusedActor(BestCandidate);}
}

void UInteractionComponent::SetFocusedActor(
	AActor* NewFocusedActor) {
	AActor* PreviousActor = FocusedActor.Get();

	if (PreviousActor == NewFocusedActor) {return;}

	/////////////////////////////////////
	// END OLD FOCUS
	/////////////////////////////////////

	if (PreviousActor &&
		PreviousActor->GetClass()->ImplementsInterface(
			UInteractableInterface::StaticClass())) {
		IInteractableInterface::Execute_OnFocusEnd(
			PreviousActor,
			GetOwner()
		);
	}
	
    /////////////////////////////////////
	// SET NEW FOCUS
	/////////////////////////////////////

	FocusedActor = NewFocusedActor;

	if (NewFocusedActor &&
		NewFocusedActor->GetClass()->ImplementsInterface(
			UInteractableInterface::StaticClass())) {
		IInteractableInterface::Execute_OnFocusStart(
			NewFocusedActor,
			GetOwner()
		);
	}

	/////////////////////////////////////
	// INTERACTOR CALLBACK
	/////////////////////////////////////

	if (GetOwner()->GetClass()->ImplementsInterface(
		UInteractorInterface::StaticClass())) {
		IInteractorInterface::Execute_OnFocusChanged(
			GetOwner(),
			PreviousActor,
			NewFocusedActor
		);
	}

	OnFocusedTargetChanged.Broadcast(
		PreviousActor,
		NewFocusedActor
	);
}

void UInteractionComponent::ClearFocus() {
	SetFocusedActor(nullptr);
}

bool UInteractionComponent::PerformInteractionTrace(
	FHitResult& OutHit) const
{
	const AActor* OwnerActor = GetOwner();

	if (!OwnerActor) {return false;}

	if (!OwnerActor->GetClass()->ImplementsInterface(
		UInteractorInterface::StaticClass())){
		return false;
	}

	const FVector Start =
		IInteractorInterface::Execute_GetInteractionOrigin(
			OwnerActor
		);

	const FVector Direction =
		IInteractorInterface::Execute_GetInteractionDirection(
			OwnerActor
		);

	const USASSettings* Settings =
		USASSettings::Get();

	const float Distance =
		OverrideInteractionDistance > 0.f
		? OverrideInteractionDistance
		: Settings->MaxInteractionDistance;

	const FVector End =
		Start + (Direction * Distance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);

	const bool bHit =
		GetWorld()->LineTraceSingleByChannel(
			OutHit,
			Start,
			End,
			Settings->InteractionTraceChannel,
			QueryParams
		);

#if !(UE_BUILD_SHIPPING)

	if (bEnableDebugDraw ||
		Settings->bDrawDetectionTrace) {
		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			bHit ? FColor::Green : FColor::Red,
			false,
			0.1f,
			0,
			1.5f
		);
	}

#endif
	return bHit;
}

AActor* UInteractionComponent::FindBestInteractionCandidate() {
	FHitResult Hit;

	if (!PerformInteractionTrace(Hit)){return nullptr;}

	AActor* HitActor = Hit.GetActor();

	if (!HitActor) {return nullptr;}

	if (!HitActor->GetClass()->ImplementsInterface(
		UInteractableInterface::StaticClass())) {
		return nullptr;
	}

	const bool bCanInteract =
		IInteractableInterface::Execute_CanInteract(
			HitActor,
			GetOwner()
		);

	if (!bCanInteract){return nullptr;}

	return HitActor;
}

bool UInteractionComponent::TryInteract()
{
	AActor* Target = FocusedActor.Get();

	if (!Target){return false;}

	if (!Target->GetClass()->ImplementsInterface(
		UInteractableInterface::StaticClass())) {
		return false;
	}

	const bool bCanInteract =
		IInteractableInterface::Execute_CanInteract(
			Target,
			GetOwner()
		);

	if (!bCanInteract) {return false;}

	CurrentInteractionTarget = Target;
	InteractionState = ESASInteractionState::Interacting;

	IInteractableInterface::Execute_OnInteractionStarted(
		Target,
		GetOwner()
	);

	if (GetOwner()->GetClass()->ImplementsInterface(
		UInteractorInterface::StaticClass())) {
		IInteractorInterface::Execute_OnInteractionStarted(
			GetOwner(),
			Target
		);
	}

	OnInteractionStarted.Broadcast(Target);

	SAS_INTERACTION_LOG(
		TEXT("%s started interaction with %s"),
		*GetNameSafe(GetOwner()),
		*GetNameSafe(Target)
	);
	return true;
}

void UInteractionComponent::CompleteInteraction()
{
	AActor* Target = CurrentInteractionTarget.Get();

	if (!Target) {return;}

	if (Target->GetClass()->ImplementsInterface(
		UInteractableInterface::StaticClass())) {
		IInteractableInterface::
			Execute_OnInteractionCompleted(
				Target,
				GetOwner()
			);
	}

	if (GetOwner()->GetClass()->ImplementsInterface(
		UInteractorInterface::StaticClass())) {
		IInteractorInterface::
			Execute_OnInteractionCompleted(
				GetOwner(),
				Target
			);
	}

	OnInteractionCompleted.Broadcast(Target);

	SAS_INTERACTION_LOG(
		TEXT("%s completed interaction with %s"),
		*GetNameSafe(GetOwner()),
		*GetNameSafe(Target)
	);

	CurrentInteractionTarget = nullptr;
	InteractionState = ESASInteractionState::Completed;
}

void UInteractionComponent::CancelInteraction()
{
	AActor* Target = CurrentInteractionTarget.Get();

	if (!Target){return;}

	if (Target->GetClass()->ImplementsInterface(
		UInteractableInterface::StaticClass())) {
		IInteractableInterface::
			Execute_OnInteractionCancelled(
				Target,
				GetOwner()
			);
	}

	if (GetOwner()->GetClass()->ImplementsInterface(
		UInteractorInterface::StaticClass())) {
		IInteractorInterface::
			Execute_OnInteractionCancelled(
				GetOwner(),
				Target
			);
	}

	OnInteractionCancelled.Broadcast(Target);

	SAS_INTERACTION_LOG(
		TEXT("%s cancelled interaction with %s"),
		*GetNameSafe(GetOwner()),
		*GetNameSafe(Target)
	);

	CurrentInteractionTarget = nullptr;
	InteractionState = ESASInteractionState::Inactive;
}

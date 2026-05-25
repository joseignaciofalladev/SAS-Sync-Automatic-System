#include "Components/InteractionProgressComponent.h"
#include "Components/InteractionComponent.h"
#include "SASLog.h"

UInteractionProgressComponent::
UInteractionProgressComponent(){
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	InteractionComponent = nullptr;
	bProgressActive = false;

	Progress = 0.f;
	CurrentTime = 0.f;
	Duration = 0.f;

	bCancelOnFocusLost = true;
	bAutoCompleteInteraction = true;
}

void
UInteractionProgressComponent::
BeginPlay(){
	Super::BeginPlay();
	InitializeInteractionComponent();
}

void
UInteractionProgressComponent::
EndPlay(
	const EEndPlayReason::Type
	EndPlayReason){
	CancelProgress();
	Super::EndPlay(EndPlayReason);
}

void
UInteractionProgressComponent::
TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction*
	ThisTickFunction){
	Super::TickComponent(
		DeltaTime,
		TickType,
		ThisTickFunction
	);

	if (!bProgressActive){return;}

	if (!TargetActor.IsValid()){
		CancelProgress();
		return;
	}

	/////////////////////////////////////
	// FOCUS VALIDATION
	/////////////////////////////////////

	if (bCancelOnFocusLost && InteractionComponent){
		AActor* FocusedActor = InteractionComponent->GetFocusedActor();

		if (FocusedActor !=TargetActor.Get()){
			CancelProgress();
			return;
		}
	}

	/////////////////////////////////////
	// UPDATE TIME
	/////////////////////////////////////

	CurrentTime += DeltaTime;

	Progress =FMath::Clamp(
			CurrentTime / Duration,
			0.f,
			1.f
		);

	OnProgressUpdated.Broadcast(TargetActor.Get(),
    Progress);

	/////////////////////////////////////
	// COMPLETE
	/////////////////////////////////////

	if (CurrentTime >= Duration){
		CompleteProgress();
	}
}

/////////////////////////////////////
// INITIALIZATION
/////////////////////////////////////

void
UInteractionProgressComponent::
InitializeInteractionComponent(){
	InteractionComponent = GetOwner() ->FindComponentByClass<UInteractionComponent>();

	if (!InteractionComponent){
		SAS_WARNING(TEXT("%s missing InteractionComponent."),*GetNameSafe(GetOwner()));
	}
}

/////////////////////////////////////
// PROGRESS
/////////////////////////////////////

bool
UInteractionProgressComponent::
StartProgress(
	AActor* InTargetActor,
	float InDuration){
	if (!InTargetActor){return false;}
	if (InDuration <= 0.f){return false;}
	if (bProgressActive){CancelProgress();}

	TargetActor = InTargetActor;
	Duration = InDuration;
	CurrentTime = 0.f;
	Progress = 0.f;
	bProgressActive = true;
	SetComponentTickEnabled(true);

	OnProgressStarted.Broadcast(InTargetActor);

	SAS_LOG(TEXT("Interaction progress started: %s"),*GetNameSafe(InTargetActor));
	return true;
}

void
UInteractionProgressComponent::
CancelProgress(){
	if (!bProgressActive){return;}

	AActor* PreviousTarget = TargetActor.Get();

	bProgressActive = false;
	Progress = 0.f;
	CurrentTime = 0.f;
	Duration = 0.f;
	TargetActor = nullptr;

	SetComponentTickEnabled(false);

	OnProgressCancelled.Broadcast(PreviousTarget);

	SAS_LOG(TEXT("Interaction progress cancelled."));
}

void
UInteractionProgressComponent::
CompleteProgress(){
	if (!bProgressActive){return;}

	AActor* CompletedTarget = TargetActor.Get();

	bProgressActive = false;
	Progress = 1.f;

	SetComponentTickEnabled(false);

	OnProgressUpdated.Broadcast(CompletedTarget,1.f);
	OnProgressCompleted.Broadcast(CompletedTarget);

	/////////////////////////////////////
	// AUTO INTERACTION
	/////////////////////////////////////

	if (bAutoCompleteInteraction && InteractionComponent){
		InteractionComponent ->Interact();
	}

	SAS_LOG(TEXT("Interaction progress completed."));

	CurrentTime = 0.f;
	Duration = 0.f;
	TargetActor = nullptr;
}

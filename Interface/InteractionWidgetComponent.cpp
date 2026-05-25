#include "UI/InteractionWidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/InteractionComponent.h"
#include "UI/SASInteractionWidget.h"
#include "SASLog.h"
#include "Components/InteractionProgressComponent.h"

UInteractionWidgetComponent::
UInteractionWidgetComponent(){
	PrimaryComponentTick.bCanEverTick = false;

	InteractionComponent = nullptr;
	InteractionWidget = nullptr;

	bWidgetVisible = false;

	bAutoShowOnFocus = true;
	bAutoHideOnFocusLost = true;
	bDestroyWidgetOnHide = false;

	InteractionProgressComponent = nullptr;
}

void
UInteractionWidgetComponent::
BeginPlay(){
	Super::BeginPlay();
	InitializeInteractionComponent();
	InitializeProgressComponent();
	CreateInteractionWidget();
}

void
UInteractionWidgetComponent::
EndPlay(
	const EEndPlayReason::Type
	EndPlayReason){
	if (InteractionComponent){
		InteractionComponent->OnFocusedTargetChanged.RemoveAll(this);
	}

	if (InteractionWidget){
		InteractionWidget->RemoveFromParent();
		InteractionWidget = nullptr;
	}

	if (InteractionProgressComponent){
		InteractionProgressComponent->OnProgressUpdated.RemoveAll(this);
		InteractionProgressComponent->OnProgressCompleted.RemoveAll(this);
		InteractionProgressComponent->OnProgressCancelled.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

/////////////////////////////////////
// INITIALIZATION
/////////////////////////////////////

void
UInteractionWidgetComponent::
InitializeInteractionComponent(){
	InteractionComponent = GetOwner()->FindComponentByClass<UInteractionComponent>();

	if (!InteractionComponent){
		SAS_WARNING(TEXT("%s missing InteractionComponent."),*GetNameSafe(GetOwner()));
		return;
	}

	InteractionComponent->OnFocusedTargetChanged.AddDynamic(this,&UInteractionWidgetComponent::HandleFocusedActorChanged);
}

void
UInteractionWidgetComponent::
InitializeProgressComponent(){
	InteractionProgressComponent = GetOwner()->FindComponentByClass<UInteractionProgressComponent>();

	if (!InteractionProgressComponent){return;}

	InteractionProgressComponent->OnProgressUpdated.AddDynamic(this,&UInteractionWidgetComponent::HandleProgressUpdated);
	InteractionProgressComponent->OnProgressCompleted.AddDynamic(this,&UInteractionWidgetComponent::HandleProgressCompleted);
	InteractionProgressComponent->OnProgressCancelled.AddDynamic(this,&UInteractionWidgetComponent::HandleProgressCancelled);
}

void
UInteractionWidgetComponent::
CreateInteractionWidget(){
	if (!InteractionWidgetClass){
		SAS_WARNING(TEXT("No InteractionWidgetClass set on %s"),*GetNameSafe(GetOwner()));
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());

	if (!PC){
		PC = GetWorld() ->GetFirstPlayerController();
	}

	if (!PC){return;}

	InteractionWidget = CreateWidget<USASInteractionWidget>(PC,InteractionWidgetClass);

	if (!InteractionWidget){return;}

	InteractionWidget->AddToViewport();
	InteractionWidget->HideWidget();

	SAS_LOG(TEXT("Interaction Widget created on %s"),*GetNameSafe(GetOwner()));
}

/////////////////////////////////////
// FOCUS EVENTS
/////////////////////////////////////

void
UInteractionWidgetComponent::
HandleFocusedActorChanged(
	AActor* PreviousTarget,
	AActor* NewTarget){
	SetTargetActor(NewTarget);

	if (NewTarget){
		if (bAutoShowOnFocus){
			ShowWidget(NewTarget);
		}
	} else {
		if (bAutoHideOnFocusLost){HideWidget();}
	}
}

void
UInteractionWidgetComponent::
HandleProgressUpdated(
	AActor* TargetActor,
	float Progress){
	if (!InteractionWidget){return;}
	InteractionWidget->SetInteractionProgress(Progress);
}

void
UInteractionWidgetComponent::
HandleProgressCompleted(
	AActor* TargetActor){
	if (!InteractionWidget){return;}
	InteractionWidget->SetInteractionProgress(1.f);
}

void
UInteractionWidgetComponent::
HandleProgressCancelled(
	AActor* TargetActor){
	if (!InteractionWidget){return;}
	InteractionWidget->SetInteractionProgress(0.f);
}

/////////////////////////////////////
// WIDGET CONTROL
/////////////////////////////////////

void
UInteractionWidgetComponent::
ShowWidget(
	AActor* TargetActor){
	if (!InteractionWidget){return;}

	CurrentTarget =TargetActor;
	InteractionWidget ->SetTargetActor(TargetActor);
	InteractionWidget ->ShowWidget();
	bWidgetVisible = true;
	OnWidgetShown.Broadcast(TargetActor);
}

void
UInteractionWidgetComponent::
HideWidget(){
	if (!InteractionWidget){return;}

	AActor* PreviousTarget = CurrentTarget.Get();

	InteractionWidget ->HideWidget();
	InteractionWidget ->ClearTargetActor();
	bWidgetVisible = false;
	CurrentTarget = nullptr;
	OnWidgetHidden.Broadcast(PreviousTarget);

	if (bDestroyWidgetOnHide){
		InteractionWidget ->RemoveFromParent();
		InteractionWidget = nullptr;
	}
}

void
UInteractionWidgetComponent::
SetTargetActor(
	AActor* NewTarget){
	if (CurrentTarget.Get()
		== NewTarget){
		return;
	}

	CurrentTarget = NewTarget;

	if (InteractionWidget){
		InteractionWidget ->SetTargetActor(NewTarget);
	} OnWidgetTargetChanged.Broadcast(NewTarget);
}

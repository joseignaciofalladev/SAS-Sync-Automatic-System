#include "Helpers/SASBlueprintLibrary.h"
#include "Components/InteractionComponent.h"
#include "Components/InteractionProgressComponent.h"
#include "Interfaces/InteractableInterface.h"
#include "Interfaces/InteractorInterface.h"

UInteractionComponent*USASBlueprintLibrary::GetInteractionComponent(AActor* Actor){
	if (!Actor){return nullptr;}
	return Actor->FindComponentByClass<UInteractionComponent>();
}

UInteractionProgressComponent*USASBlueprintLibrary::GetInteractionProgressComponent(AActor* Actor){
	if (!Actor){return nullptr;}
	return Actor -> FindComponentByClass<UInteractionProgressComponent>();
}

/////////////////////////////////////
// FOCUSED TARGET
/////////////////////////////////////

AActor*USASBlueprintLibrary::GetFocusedInteractionActor(AActor* Actor){
	if (!Actor){return nullptr;}
	UInteractionComponent*InteractionComponent = GetInteractionComponent(Actor);

	if (!InteractionComponent){return nullptr;}
	return InteractionComponent->GetFocusedActor();
}

/////////////////////////////////////
// INTERACTION
/////////////////////////////////////

bool USASBlueprintLibrary:Interact(AActor* Actor){
	if (!Actor){return false;}
	UInteractionComponent*InteractionComponent = GetInteractionComponent(Actor);

	if (!InteractionComponent){return false;}

	InteractionComponent->Interact();
	return true;
}

bool USASBlueprintLibrary::StartTimedInteraction(AActor* Actor,float Duration){
	if (!Actor){return false;}
	if (Duration <= 0.f){return false;}

	UInteractionComponent*InteractionComponent = GetInteractionComponent(Actor);

	if (!InteractionComponent){return false;}
	UInteractionProgressComponent*ProgressComponent = GetInteractionProgressComponent(Actor);

	if (!ProgressComponent){return false;}
	AActor* FocusedActor =InteractionComponent->GetFocusedActor();

	if (!FocusedActor){return false;}
	return ProgressComponent->StartProgress(FocusedActor,Duration);
}

void USASBlueprintLibrary::CancelTimedInteraction(AActor* Actor){
	if (!Actor){return;}
	UInteractionProgressComponent*ProgressComponent = GetInteractionProgressComponent(Actor);

    if (!ProgressComponent){return;}
	ProgressComponent->CancelProgress();
}

/////////////////////////////////////
// VALIDATION
/////////////////////////////////////

bool USASBlueprintLibrary::IsInteractable(AActor* Actor){
	if (!Actor){return false;}
	return Actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass());
}

bool USASBlueprintLibrary::IsInteractor(AActor* Actor){
	if (!Actor){return false;}
	return Actor->GetClass()->ImplementsInterface(UInteractorInterface::StaticClass());
}

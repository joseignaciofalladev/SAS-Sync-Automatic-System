#include "UI/SASInteractionWidget.h"
#include "Components/Widget.h"

USASInteractionWidget::USASInteractionWidget(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer){
	InteractionProgress = 0.f;
	bVisible = false;
}

void USASInteractionWidget::NativeConstruct(){
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
}

void USASInteractionWidget::NativeDestruct(){
	TargetActor = nullptr;
	Super::NativeDestruct();
}

/////////////////////////////////////
// TARGET
/////////////////////////////////////
void USASInteractionWidget::SetTargetActor(
	AActor* NewTargetActor){
	if (TargetActor.Get() == NewTargetActor){
		return;
	}

	TargetActor = NewTargetActor;
	BP_OnTargetChanged(NewTargetActor);
}

void USASInteractionWidget::ClearTargetActor(){
	if (!TargetActor.IsValid()){return;}

	TargetActor = nullptr;
	InteractionProgress = 0.f;
	BP_OnTargetCleared();
}

/////////////////////////////////////
// VISIBILITY
/////////////////////////////////////
void USASInteractionWidget::ShowWidget(){
	if (bVisible){return;}
	bVisible = true;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	BP_OnWidgetShown();
}

void USASInteractionWidget::HideWidget(){
	if (!bVisible){return;}
	bVisible = false;
	SetVisibility(ESlateVisibility::Collapsed);
	InteractionProgress = 0.f;
	BP_OnWidgetHidden();
}

void USASInteractionWidget::SetInteractionProgress(
	float NewProgress){
	const float ClampedProgress = FMath::Clamp(NewProgress,0.f,1.f);

	if (FMath::IsNearlyEqual(InteractionProgress,ClampedProgress)){
		return;
	}

	InteractionProgress = ClampedProgress;
	BP_OnProgressUpdated(InteractionProgress);
}

#pragma once

#include "SASSettings.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

USASSettings::USASSettings()
{
	// Detection
	MaxInteractionDistance = 300.f;
	DetectionInterval = 0.05f;
	InteractionTraceChannel = ECC_Visibility;
	bUseSphereFallback = true;
	DetectionRadius = 45.f;

	// Focus
	bEnableAutoFocus = true;
	FocusRefreshRate = 0.05f;
	MinimumFocusScore = 5.f;

	// Interaction
	bAllowConcurrentInteractions = false;
	bAllowInteractionReplacement = true;
	InteractionTimeout = 3.f;

	// Networking
	bEnableClientPrediction = true;
	bUseServerAuthority = true;

	// Debug
	bEnableDebugDraw = false;
	bDrawDetectionTrace = false;
	bVerboseLogging = false;
}

// SAS runtime module
class FSASModule final : public IModuleInterface {
public:
	// IMODULE INTERFACE
	virtual void StartupModule()
		override;

	virtual void ShutdownModule()
		override;
private:
	// Registers gameplay tags
	void InitializeGameplayTags();
};

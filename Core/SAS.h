#include "SASSettings.h"

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
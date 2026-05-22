#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/EngineTypes.h"
#include "SASSettings.generated.h"

/**
 * Global project settings for SAS (Sync Automatic System).
 * Accessible through:
 * Project Settings → Plugins → SAS
 * Stores framework-wide defaults for interaction,
 * detection, networking and debugging.
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="SAS Settings"))
class SAS_API USASSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	USASSettings();

	/////////////////////////////////////
	// DEVELOPER SETTINGS OVERRIDES
	/////////////////////////////////////

	virtual FName GetCategoryName() const override
	{return TEXT("Plugins");}

#if WITH_EDITOR

	virtual FText GetSectionText() const override
	{
		return NSLOCTEXT(
			"SAS",
			"SASSettingsSection",
			"Sync Automatic System"
		);
	}

	virtual FText GetSectionDescription() const override
	{
		return NSLOCTEXT(
			"SAS",
			"SASSettingsDescription",
			"Global configuration for SAS interaction framework."
		);
	}

#endif

	/////////////////////////////////////
	// DETECTION
	/////////////////////////////////////

	// Maximum interaction trace distance
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection",
		meta=(ClampMin="50.0", ClampMax="10000.0")
	)
	float MaxInteractionDistance;

	// Frequency of interaction detection checks
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection",
		meta=(ClampMin="0.01", ClampMax="1.0")
	)
	float DetectionInterval;

	// Collision channel used for interaction traces 
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection"
	)
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

	// Allow sphere sweep fallback if line trace fails
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection"
	)
	bool bUseSphereFallback;

	// Radius used for sphere interaction checks
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection",
		meta=(ClampMin="1.0", ClampMax="500.0")
	)
	float DetectionRadius;

	/////////////////////////////////////
	// FOCUS
	/////////////////////////////////////

	// Enable automatic interaction focus
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Focus"
	)
	bool bEnableAutoFocus;

	// Time before focus target refresh
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Focus",
		meta=(ClampMin="0.01", ClampMax="1.0")
	)
	float FocusRefreshRate;

	// Minimum score required to receive focus
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Focus",
		meta=(ClampMin="0.0", ClampMax="1000.0")
	)
	float MinimumFocusScore;

	/////////////////////////////////////
	// INTERACTION
	/////////////////////////////////////

	// Allow multiple simultaneous interactions
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Interaction"
	)
	bool bAllowConcurrentInteractions;

	// Cancel current interaction when better target exists
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Interaction"
	)
	bool bAllowInteractionReplacement;

	// Time before interaction expires
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Interaction",
		meta=(ClampMin="0.0", ClampMax="30.0")
	)
	float InteractionTimeout;

	/////////////////////////////////////
	// NETWORKING
	/////////////////////////////////////

	// Enable client-side prediction
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Networking"
	)
	bool bEnableClientPrediction;

	//Use server authority validation
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Networking"
	)
	bool bUseServerAuthority;

	/////////////////////////////////////
	// DEBUG
	/////////////////////////////////////

	// Enable debug drawing
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Debug"
	)
	bool bEnableDebugDraw;

	// Draw interaction traces
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Debug"
	)
	bool bDrawDetectionTrace;

	// Print verbose SAS logs
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Debug"
	)
	bool bVerboseLogging;

public:

	// Returns mutable SAS settings object
	UFUNCTION(BlueprintPure, Category="SAS")
	static const USASSettings* Get()
	{return GetDefault<USASSettings>();}
};
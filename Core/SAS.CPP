#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/EngineTypes.h"
#include "SASSettings.generated.h"
#include "SAS.h"
#include "SASGameplayTags.h"
#include "SASLog.h"

#define LOCTEXT_NAMESPACE "FSASModule"

/**
 * Global SAS configuration.
 * Accessible from:
 * Project Settings → Plugins → SAS
 * Stores framework-wide defaults for:
 * - Detection
 * - Focus
 * - Interaction
 * - Networking
 * - Debugging
 */
UCLASS(
	Config=Game,
	DefaultConfig,
	meta=(DisplayName="SAS")
)
class SAS_API USASSettings final : public UDeveloperSettings
{
	GENERATED_BODY()
public:

void FSASModule::StartupModule()
{
	SAS_LOG(TEXT("Starting SAS Module..."));
	InitializeGameplayTags();
	SAS_LOG(TEXT("SAS Module started successfully."));
}

void FSASModule::ShutdownModule(){
	SAS_LOG(TEXT("Shutting down SAS Module..."));
	SAS_LOG(TEXT("SAS Module shutdown complete."));
}

void FSASModule::InitializeGameplayTags(){
	FSASGameplayTags:: InitializeNativeTags();
}

IMPLEMENT_MODULE(
	FSASModule,
	SAS
)

#undef LOCTEXT_NAMESPACE
	USASSettings();

	/////////////////////////////////////
	// DEVELOPER SETTINGS
	/////////////////////////////////////
	virtual FName GetCategoryName() const override
	{return TEXT("Plugins");}
#if WITH_EDITOR
	virtual FText GetSectionText() const override
	{
		return NSLOCTEXT(
			"SAS",
			"SASSectionName",
			"Sync Automatic System"
		);
	}

	virtual FText GetSectionDescription() const override
	{
		return NSLOCTEXT(
			"SAS",
			"SASSectionDescription",
			"Global settings for the SAS interaction framework."
		);
	}
#endif
	/////////////////////////////////////
	// DETECTION
	/////////////////////////////////////
	// Maximum interaction detection distance
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection",
		meta=(
			ClampMin="50.0",
			UIMin="50.0",
			UIMax="1000.0"
		)
	)
	float MaxInteractionDistance;

	// Detection update interval
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection",
		meta=(
			ClampMin="0.01",
			UIMin="0.01",
			UIMax="0.25"
		)
	)
	float DetectionInterval;

	// Trace channel used by interaction detection
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection"
	)
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

	// Fallback sphere sweep if trace misses
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection"
	)
	bool bUseSphereFallback;

	// Radius used for overlap detection
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Detection",
		meta=(
			ClampMin="1.0",
			UIMin="1.0",
			UIMax="150.0"
		)
	)
	float DetectionRadius;

	/////////////////////////////////////
	// FOCUS
	/////////////////////////////////////
	// Automatically focus best interaction candidate
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Focus"
	)
	bool bEnableAutoFocus;

	// Focus refresh frequency
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Focus",
		meta=(
			ClampMin="0.01",
			UIMin="0.01",
			UIMax="0.25"
		)
	)
	float FocusRefreshRate;

	// Minimum score required for focus
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Focus",
		meta=(
			ClampMin="0.0",
			UIMin="0.0",
			UIMax="100.0"
		)
	)
	float MinimumFocusScore;

	/////////////////////////////////////
	// INTERACTION
	/////////////////////////////////////
	// Allow multiple active interactions
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Interaction"
	)
	bool bAllowConcurrentInteractions;

	// Replace interaction when better candidate appears
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Interaction"
	)
	bool bAllowInteractionReplacement;

	// Interaction timeout in seconds
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Interaction",
		meta=(
			ClampMin="0.0",
			UIMin="0.0",
			UIMax="30.0"
		)
	)
	float InteractionTimeout;

	/////////////////////////////////////
	// NETWORKING
	/////////////////////////////////////
	// Enables client prediction
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Networking"
	)
	bool bEnableClientPrediction;

	// Server validates interaction authority
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
	// Enables debug drawing
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

	// Enables verbose SAS logging
	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Debug"
	)
	bool bVerboseLogging;
public:
	UFUNCTION(BlueprintPure, Category="SAS")
	static FORCEINLINE const USASSettings* Get()
	{return GetDefault<USASSettings>();}
};

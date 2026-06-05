#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/EngineTypes.h"
#include "SASTypes.generated.h"

/////////////////////////////////////
// ENUMS
/////////////////////////////////////

// Defines how an interaction is triggered
UENUM(BlueprintType)
enum class ESASInteractionInputType : uint8
{
	Press		UMETA(DisplayName = "Press"),
	Hold		UMETA(DisplayName = "Hold"),
	Toggle		UMETA(DisplayName = "Toggle"),
	Automatic	UMETA(DisplayName = "Automatic")
};

// Defines the lifecycle state of an interaction
UENUM(BlueprintType)
enum class ESASInteractionState : uint8
{
	Inactive	UMETA(DisplayName = "Inactive"),
	Available	UMETA(DisplayName = "Available"),
	Focused	UMETA(DisplayName = "Focused"),
	Interacting UMETA(DisplayName = "Interacting"),
	Completed	UMETA(DisplayName = "Completed"),
	Blocked		UMETA(DisplayName = "Blocked"),
	Disabled	UMETA(DisplayName = "Disabled")
};

// Defines how the interaction should be resolved
UENUM(BlueprintType)
enum class ESASInteractionMode : uint8
{
	Instant		UMETA(DisplayName = "Instant"),
	Timed		UMETA(DisplayName = "Timed"),
	Continuous UMETA(DisplayName = "Continuous")
};

// Defines authority behavior for multiplayer interactions
UENUM(BlueprintType)
enum class ESASInteractionAuthority : uint8
{
	ServerOnly		UMETA(DisplayName = "Server Only"),
	ClientPredicted UMETA(DisplayName = "Client Predicted"),
	LocalOnly		UMETA(DisplayName = "Local Only")
};

/////////////////////////////////////
// STRUCTS
/////////////////////////////////////
/**
 * Context information generated when evaluating interactions.
 * This structure is passed through the framework to score and
 * resolve the best interaction candidate.
 */
USTRUCT(BlueprintType)
struct FSASInteractionContext
{
	GENERATED_BODY()
public:
	FSASInteractionContext()
		: Instigator(nullptr)
		, Target(nullptr)
		, Distance(0.f)
		, Score(0.f)
		, bIsVisible(false)
	{}

	// Actor requesting interaction
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") AActor* Instigator;
	// Actor being interacted with
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") AActor* Target;
	// Gameplay tags used for contextual filtering
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") FGameplayTagContainer ContextTags;
	// Distance between interactor and target
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") float Distance;
	// Final calculated interaction score
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") float Score;
	// Whether target is visible from the interactor
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") bool bIsVisible;
};

// Interaction request sent by an interactor
USTRUCT(BlueprintType)
struct FSASInteractionRequest
{
	GENERATED_BODY()
public:
	FSASInteractionRequest()
		: Interactor(nullptr)
		, Target(nullptr)
		, InputType(ESASInteractionInputType::Press)
	{}
	UPROPERTY(BlueprintReadWrite, Category="SAS|Interaction") AActor* Interactor;
	UPROPERTY(BlueprintReadWrite, Category="SAS|Interaction") AActor* Target;
	UPROPERTY(BlueprintReadWrite, Category="SAS|Interaction") ESASInteractionInputType InputType;
	UPROPERTY(BlueprintReadWrite, Category="SAS|Interaction") FGameplayTagContainer RequestTags;
};

// Interaction result returned by the system
USTRUCT(BlueprintType)
struct FSASInteractionResult
{
	GENERATED_BODY()
public:
	FSASInteractionResult()
		: bSuccess(false)
		, State(ESASInteractionState::Inactive)
	{}

	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") bool bSuccess;
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") ESASInteractionState State;
	UPROPERTY(BlueprintReadOnly, Category="SAS|Interaction") FText Message;
};


// Scoring data used by the interaction resolver
USTRUCT(BlueprintType)
struct FSASInteractionScore
{
	GENERATED_BODY()
public:

	FSASInteractionScore()
		: BaseScore(0.f)
		, DistanceWeight(1.f)
		, VisibilityWeight(1.f)
		, PriorityWeight(1.f)
		, FinalScore(0.f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Scoring") float BaseScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Scoring") float DistanceWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Scoring") float VisibilityWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SAS|Scoring") float PriorityWeight;
	UPROPERTY(BlueprintReadOnly, Category="SAS|Scoring") float FinalScore;
};

/////////////////////////////////////
// DELEGATES
/////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FSASInteractionStateChangedSignature,
	ESASInteractionState,
	NewState
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FSASInteractionStartedSignature,
	AActor*,
	Interactor,
	AActor*,
	Target
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FSASInteractionCompletedSignature,
	AActor*,
	Interactor,
	AActor*,
	Target
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FSASInteractionCancelledSignature,
	AActor*,
	Interactor,
	AActor*,
	Target
);

#pragma once

#include "CoreMinimal.h"

/////////////////////////////////////
// LOG CATEGORIES
/////////////////////////////////////
DECLARE_LOG_CATEGORY_EXTERN(LogSAS, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSASInteraction, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSASNetwork, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSASDebug, Log, All);

/////////////////////////////////////
// INTERNAL HELPERS
/////////////////////////////////////
#define SAS_FUNCTION_NAME TEXT(__FUNCTION__)

#define SAS_LOG_INTERNAL(Category, Verbosity, Format, ...) \
	UE_LOG( \
		Category, \
		Verbosity, \
		TEXT("[%s][Line: %d] ") Format, \
		SAS_FUNCTION_NAME, \
		__LINE__, \
		##__VA_ARGS__ \
	)

/////////////////////////////////////
// GENERAL LOGGING
/////////////////////////////////////
#define SAS_LOG(Format, ...) \ SAS_LOG_INTERNAL(LogSAS, Log, Format, ##__VA_ARGS__)
#define SAS_WARNING(Format, ...) \ SAS_LOG_INTERNAL(LogSAS, Warning, Format, ##__VA_ARGS__)
#define SAS_ERROR(Format, ...) \ SAS_LOG_INTERNAL(LogSAS, Error, Format, ##__VA_ARGS__)

/////////////////////////////////////
// INTERACTION LOGGING
/////////////////////////////////////
#define SAS_INTERACTION_LOG(Format, ...) \ SAS_LOG_INTERNAL(LogSASInteraction, Log, Format, ##__VA_ARGS__)
#define SAS_INTERACTION_WARNING(Format, ...) \ SAS_LOG_INTERNAL(LogSASInteraction, Warning, Format, ##__VA_ARGS__)
#define SAS_INTERACTION_ERROR(Format, ...) \ SAS_LOG_INTERNAL(LogSASInteraction, Error, Format, ##__VA_ARGS__)

/////////////////////////////////////
// NETWORK LOGGING
/////////////////////////////////////
#define SAS_NETWORK_LOG(Format, ...) \ SAS_LOG_INTERNAL(LogSASNetwork, Log, Format, ##__VA_ARGS__)
#define SAS_NETWORK_WARNING(Format, ...) \ SAS_LOG_INTERNAL(LogSASNetwork, Warning, Format, ##__VA_ARGS__)
#define SAS_NETWORK_ERROR(Format, ...) \ SAS_LOG_INTERNAL(LogSASNetwork, Error, Format, ##__VA_ARGS__)

/////////////////////////////////////
// DEBUG LOGGING
/////////////////////////////////////
#if !UE_BUILD_SHIPPING
	#define SAS_DEBUG_LOG(Format, ...) \ SAS_LOG_INTERNAL(LogSASDebug, Log, Format, ##__VA_ARGS__)
	#define SAS_DEBUG_WARNING(Format, ...) \ SAS_LOG_INTERNAL(LogSASDebug, Warning, Format, ##__VA_ARGS__)
#else
	#define SAS_DEBUG_LOG(...)
	#define SAS_DEBUG_WARNING(...)
#endif

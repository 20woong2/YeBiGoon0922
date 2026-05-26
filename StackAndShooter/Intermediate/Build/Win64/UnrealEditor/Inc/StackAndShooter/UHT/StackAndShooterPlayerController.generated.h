// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "StackAndShooterPlayerController.h"

#ifdef STACKANDSHOOTER_StackAndShooterPlayerController_generated_h
#error "StackAndShooterPlayerController.generated.h already included, missing '#pragma once' in StackAndShooterPlayerController.h"
#endif
#define STACKANDSHOOTER_StackAndShooterPlayerController_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AStackAndShooterPlayerController *****************************************
STACKANDSHOOTER_API UClass* Z_Construct_UClass_AStackAndShooterPlayerController_NoRegister();

#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterPlayerController_h_20_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAStackAndShooterPlayerController(); \
	friend struct Z_Construct_UClass_AStackAndShooterPlayerController_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend STACKANDSHOOTER_API UClass* Z_Construct_UClass_AStackAndShooterPlayerController_NoRegister(); \
public: \
	DECLARE_CLASS2(AStackAndShooterPlayerController, APlayerController, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/StackAndShooter"), Z_Construct_UClass_AStackAndShooterPlayerController_NoRegister) \
	DECLARE_SERIALIZER(AStackAndShooterPlayerController)


#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterPlayerController_h_20_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AStackAndShooterPlayerController(AStackAndShooterPlayerController&&) = delete; \
	AStackAndShooterPlayerController(const AStackAndShooterPlayerController&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AStackAndShooterPlayerController); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AStackAndShooterPlayerController); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(AStackAndShooterPlayerController) \
	NO_API virtual ~AStackAndShooterPlayerController();


#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterPlayerController_h_17_PROLOG
#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterPlayerController_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterPlayerController_h_20_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterPlayerController_h_20_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AStackAndShooterPlayerController;

// ********** End Class AStackAndShooterPlayerController *******************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterPlayerController_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS

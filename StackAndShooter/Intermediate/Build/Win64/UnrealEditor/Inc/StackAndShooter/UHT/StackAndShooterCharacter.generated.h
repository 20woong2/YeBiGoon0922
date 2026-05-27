// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "StackAndShooterCharacter.h"

#ifdef STACKANDSHOOTER_StackAndShooterCharacter_generated_h
#error "StackAndShooterCharacter.generated.h already included, missing '#pragma once' in StackAndShooterCharacter.h"
#endif
#define STACKANDSHOOTER_StackAndShooterCharacter_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AStackAndShooterCharacter ************************************************
#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execDoJumpEnd); \
	DECLARE_FUNCTION(execDoJumpStart); \
	DECLARE_FUNCTION(execDoMove); \
	DECLARE_FUNCTION(execDoAim);


STACKANDSHOOTER_API UClass* Z_Construct_UClass_AStackAndShooterCharacter_NoRegister();

#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_24_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAStackAndShooterCharacter(); \
	friend struct Z_Construct_UClass_AStackAndShooterCharacter_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend STACKANDSHOOTER_API UClass* Z_Construct_UClass_AStackAndShooterCharacter_NoRegister(); \
public: \
	DECLARE_CLASS2(AStackAndShooterCharacter, ACharacter, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/StackAndShooter"), Z_Construct_UClass_AStackAndShooterCharacter_NoRegister) \
	DECLARE_SERIALIZER(AStackAndShooterCharacter)


#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_24_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AStackAndShooterCharacter(AStackAndShooterCharacter&&) = delete; \
	AStackAndShooterCharacter(const AStackAndShooterCharacter&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AStackAndShooterCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AStackAndShooterCharacter); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(AStackAndShooterCharacter) \
	NO_API virtual ~AStackAndShooterCharacter();


#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_21_PROLOG
#define FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_24_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_24_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h_24_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AStackAndShooterCharacter;

// ********** End Class AStackAndShooterCharacter **************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_StackAndShooter_Source_StackAndShooter_StackAndShooterCharacter_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS

// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ShooterGameMode.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeShooterGameMode() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
STACKANDSHOOTER_API UClass* Z_Construct_UClass_AShooterGameMode();
STACKANDSHOOTER_API UClass* Z_Construct_UClass_AShooterGameMode_NoRegister();
STACKANDSHOOTER_API UClass* Z_Construct_UClass_UShooterUI_NoRegister();
UPackage* Z_Construct_UPackage__Script_StackAndShooter();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AShooterGameMode Function IncrementTeamScore *****************************
struct Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics
{
	struct ShooterGameMode_eventIncrementTeamScore_Parms
	{
		uint8 TeamByte;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Score" },
		{ "ModuleRelativePath", "Variant_Shooter/ShooterGameMode.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_TeamByte;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::NewProp_TeamByte = { "TeamByte", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ShooterGameMode_eventIncrementTeamScore_Parms, TeamByte), nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::NewProp_TeamByte,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AShooterGameMode, nullptr, "IncrementTeamScore", Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::PropPointers), sizeof(Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::ShooterGameMode_eventIncrementTeamScore_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::Function_MetaDataParams), Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::ShooterGameMode_eventIncrementTeamScore_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AShooterGameMode::execIncrementTeamScore)
{
	P_GET_PROPERTY(FByteProperty,Z_Param_TeamByte);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->IncrementTeamScore(Z_Param_TeamByte);
	P_NATIVE_END;
}
// ********** End Class AShooterGameMode Function IncrementTeamScore *******************************

// ********** Begin Class AShooterGameMode Function OnScoreIncremented *****************************
struct ShooterGameMode_eventOnScoreIncremented_Parms
{
	uint8 TeamId;
	int32 NewScore;
};
static FName NAME_AShooterGameMode_OnScoreIncremented = FName(TEXT("OnScoreIncremented"));
void AShooterGameMode::OnScoreIncremented(uint8 TeamId, int32 NewScore)
{
	ShooterGameMode_eventOnScoreIncremented_Parms Parms;
	Parms.TeamId=TeamId;
	Parms.NewScore=NewScore;
	UFunction* Func = FindFunctionChecked(NAME_AShooterGameMode_OnScoreIncremented);
	ProcessEvent(Func,&Parms);
}
struct Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Score" },
		{ "ModuleRelativePath", "Variant_Shooter/ShooterGameMode.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_TeamId;
	static const UECodeGen_Private::FIntPropertyParams NewProp_NewScore;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::NewProp_TeamId = { "TeamId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ShooterGameMode_eventOnScoreIncremented_Parms, TeamId), nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::NewProp_NewScore = { "NewScore", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ShooterGameMode_eventOnScoreIncremented_Parms, NewScore), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::NewProp_TeamId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::NewProp_NewScore,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AShooterGameMode, nullptr, "OnScoreIncremented", Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::PropPointers), sizeof(ShooterGameMode_eventOnScoreIncremented_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::Function_MetaDataParams), Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(ShooterGameMode_eventOnScoreIncremented_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class AShooterGameMode Function OnScoreIncremented *******************************

// ********** Begin Class AShooterGameMode *********************************************************
void AShooterGameMode::StaticRegisterNativesAShooterGameMode()
{
	UClass* Class = AShooterGameMode::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "IncrementTeamScore", &AShooterGameMode::execIncrementTeamScore },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_AShooterGameMode;
UClass* AShooterGameMode::GetPrivateStaticClass()
{
	using TClass = AShooterGameMode;
	if (!Z_Registration_Info_UClass_AShooterGameMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("ShooterGameMode"),
			Z_Registration_Info_UClass_AShooterGameMode.InnerSingleton,
			StaticRegisterNativesAShooterGameMode,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AShooterGameMode.InnerSingleton;
}
UClass* Z_Construct_UClass_AShooterGameMode_NoRegister()
{
	return AShooterGameMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AShooterGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  Simple GameMode for a first person shooter game\n *  Manages game UI\n *  Keeps track of team scores\n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Variant_Shooter/ShooterGameMode.h" },
		{ "ModuleRelativePath", "Variant_Shooter/ShooterGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Simple GameMode for a first person shooter game\nManages game UI\nKeeps track of team scores" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShooterUIClass_MetaData[] = {
		{ "Category", "Shooter" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Type of UI widget to spawn */" },
#endif
		{ "ModuleRelativePath", "Variant_Shooter/ShooterGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Type of UI widget to spawn" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_ShooterUIClass;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AShooterGameMode_IncrementTeamScore, "IncrementTeamScore" }, // 239007355
		{ &Z_Construct_UFunction_AShooterGameMode_OnScoreIncremented, "OnScoreIncremented" }, // 3326915109
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AShooterGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_AShooterGameMode_Statics::NewProp_ShooterUIClass = { "ShooterUIClass", nullptr, (EPropertyFlags)0x0024080000000001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AShooterGameMode, ShooterUIClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UShooterUI_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShooterUIClass_MetaData), NewProp_ShooterUIClass_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AShooterGameMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AShooterGameMode_Statics::NewProp_ShooterUIClass,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AShooterGameMode_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AShooterGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_StackAndShooter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AShooterGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AShooterGameMode_Statics::ClassParams = {
	&AShooterGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AShooterGameMode_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AShooterGameMode_Statics::PropPointers),
	0,
	0x009003ADu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AShooterGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AShooterGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AShooterGameMode()
{
	if (!Z_Registration_Info_UClass_AShooterGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AShooterGameMode.OuterSingleton, Z_Construct_UClass_AShooterGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AShooterGameMode.OuterSingleton;
}
AShooterGameMode::AShooterGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(AShooterGameMode);
AShooterGameMode::~AShooterGameMode() {}
// ********** End Class AShooterGameMode ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_user_Documents_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_Variant_Shooter_ShooterGameMode_h__Script_StackAndShooter_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AShooterGameMode, AShooterGameMode::StaticClass, TEXT("AShooterGameMode"), &Z_Registration_Info_UClass_AShooterGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AShooterGameMode), 2669002135U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_user_Documents_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_Variant_Shooter_ShooterGameMode_h__Script_StackAndShooter_31403529(TEXT("/Script/StackAndShooter"),
	Z_CompiledInDeferFile_FID_Users_user_Documents_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_Variant_Shooter_ShooterGameMode_h__Script_StackAndShooter_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_user_Documents_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_Variant_Shooter_ShooterGameMode_h__Script_StackAndShooter_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS

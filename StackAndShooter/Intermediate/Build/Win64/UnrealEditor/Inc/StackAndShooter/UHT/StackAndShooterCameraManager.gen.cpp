// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "StackAndShooterCameraManager.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeStackAndShooterCameraManager() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_APlayerCameraManager();
STACKANDSHOOTER_API UClass* Z_Construct_UClass_AStackAndShooterCameraManager();
STACKANDSHOOTER_API UClass* Z_Construct_UClass_AStackAndShooterCameraManager_NoRegister();
UPackage* Z_Construct_UPackage__Script_StackAndShooter();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AStackAndShooterCameraManager ********************************************
void AStackAndShooterCameraManager::StaticRegisterNativesAStackAndShooterCameraManager()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_AStackAndShooterCameraManager;
UClass* AStackAndShooterCameraManager::GetPrivateStaticClass()
{
	using TClass = AStackAndShooterCameraManager;
	if (!Z_Registration_Info_UClass_AStackAndShooterCameraManager.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("StackAndShooterCameraManager"),
			Z_Registration_Info_UClass_AStackAndShooterCameraManager.InnerSingleton,
			StaticRegisterNativesAStackAndShooterCameraManager,
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
	return Z_Registration_Info_UClass_AStackAndShooterCameraManager.InnerSingleton;
}
UClass* Z_Construct_UClass_AStackAndShooterCameraManager_NoRegister()
{
	return AStackAndShooterCameraManager::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AStackAndShooterCameraManager_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  Basic First Person camera manager.\n *  Limits min/max look pitch.\n */" },
#endif
		{ "IncludePath", "StackAndShooterCameraManager.h" },
		{ "ModuleRelativePath", "StackAndShooterCameraManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Basic First Person camera manager.\nLimits min/max look pitch." },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AStackAndShooterCameraManager>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AStackAndShooterCameraManager_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerCameraManager,
	(UObject* (*)())Z_Construct_UPackage__Script_StackAndShooter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AStackAndShooterCameraManager_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AStackAndShooterCameraManager_Statics::ClassParams = {
	&AStackAndShooterCameraManager::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008003ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AStackAndShooterCameraManager_Statics::Class_MetaDataParams), Z_Construct_UClass_AStackAndShooterCameraManager_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AStackAndShooterCameraManager()
{
	if (!Z_Registration_Info_UClass_AStackAndShooterCameraManager.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AStackAndShooterCameraManager.OuterSingleton, Z_Construct_UClass_AStackAndShooterCameraManager_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AStackAndShooterCameraManager.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AStackAndShooterCameraManager);
AStackAndShooterCameraManager::~AStackAndShooterCameraManager() {}
// ********** End Class AStackAndShooterCameraManager **********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Unreal_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_StackAndShooterCameraManager_h__Script_StackAndShooter_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AStackAndShooterCameraManager, AStackAndShooterCameraManager::StaticClass, TEXT("AStackAndShooterCameraManager"), &Z_Registration_Info_UClass_AStackAndShooterCameraManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AStackAndShooterCameraManager), 3696826156U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_StackAndShooterCameraManager_h__Script_StackAndShooter_2866346067(TEXT("/Script/StackAndShooter"),
	Z_CompiledInDeferFile_FID_Unreal_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_StackAndShooterCameraManager_h__Script_StackAndShooter_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_YeBiGoon0922_StackAndShooter_Source_StackAndShooter_StackAndShooterCameraManager_h__Script_StackAndShooter_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS

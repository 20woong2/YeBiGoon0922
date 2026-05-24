// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStackAndShooter_init() {}
	STACKANDSHOOTER_API UFunction* Z_Construct_UDelegateFunction_StackAndShooter_BulletCountUpdatedDelegate__DelegateSignature();
	STACKANDSHOOTER_API UFunction* Z_Construct_UDelegateFunction_StackAndShooter_DamagedDelegate__DelegateSignature();
	STACKANDSHOOTER_API UFunction* Z_Construct_UDelegateFunction_StackAndShooter_PawnDeathDelegate__DelegateSignature();
	STACKANDSHOOTER_API UFunction* Z_Construct_UDelegateFunction_StackAndShooter_SprintStateChangedDelegate__DelegateSignature();
	STACKANDSHOOTER_API UFunction* Z_Construct_UDelegateFunction_StackAndShooter_UpdateSprintMeterDelegate__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_StackAndShooter;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_StackAndShooter()
	{
		if (!Z_Registration_Info_UPackage__Script_StackAndShooter.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_StackAndShooter_BulletCountUpdatedDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_StackAndShooter_DamagedDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_StackAndShooter_PawnDeathDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_StackAndShooter_SprintStateChangedDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_StackAndShooter_UpdateSprintMeterDelegate__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/StackAndShooter",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0xFA113EF6,
				0x83E945AA,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_StackAndShooter.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_StackAndShooter.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_StackAndShooter(Z_Construct_UPackage__Script_StackAndShooter, TEXT("/Script/StackAndShooter"), Z_Registration_Info_UPackage__Script_StackAndShooter, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xFA113EF6, 0x83E945AA));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "ShooterProjectile.h"
#include "ShooterWeaponHolder.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"

AShooterWeapon::AShooterWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// create the first person mesh
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(RootComponent);

	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	FirstPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	FirstPersonMesh->bOnlyOwnerSee = true;

	// create the third person mesh
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Third Person Mesh"));
	ThirdPersonMesh->SetupAttachment(RootComponent);

	ThirdPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	ThirdPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	ThirdPersonMesh->bOwnerNoSee = true;
}

void AShooterWeapon::BeginPlay()
{
	Super::BeginPlay();

	// subscribe to the owner's destroyed delegate
	GetOwner()->OnDestroyed.AddDynamic(this, &AShooterWeapon::OnOwnerDestroyed);

	// cast the weapon owner
	WeaponOwner = Cast<IShooterWeaponHolder>(GetOwner());
	PawnOwner = Cast<APawn>(GetOwner());

	// fill the first ammo clip
	CurrentBullets = MagazineSize;

	// attach the meshes to the owner
	WeaponOwner->AttachWeaponMeshes(this);
}

void AShooterWeapon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the refire timer
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

void AShooterWeapon::OnOwnerDestroyed(AActor* DestroyedActor)
{
	// ensure this weapon is destroyed when the owner is destroyed
	Destroy();
}

void AShooterWeapon::ActivateWeapon()
{
	// unhide this weapon
	SetActorHiddenInGame(false);

	// notify the owner
	WeaponOwner->OnWeaponActivated(this);
}

void AShooterWeapon::DeactivateWeapon()
{
	// ensure we're no longer firing this weapon while deactivated
	StopFiring();

	// hide the weapon
	SetActorHiddenInGame(true);

	// notify the owner
	WeaponOwner->OnWeaponDeactivated(this);
}

void AShooterWeapon::StartFiring()
{
	// raise the firing flag
	bIsFiring = true;

	// check how much time has passed since we last shot
	// this may be under the refire rate if the weapon shoots slow enough and the player is spamming the trigger
	const float TimeSinceLastShot = GetWorld()->GetTimeSeconds() - TimeOfLastShot;

	if (TimeSinceLastShot > RefireRate)
	{
		// fire the weapon right away
		Fire();

	} else {

		// if we're full auto, schedule the next shot
		if (bFullAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AShooterWeapon::Fire, TimeSinceLastShot, false);
		}

	}
}

void AShooterWeapon::StopFiring()
{
	// lower the firing flag
	bIsFiring = false;

	// clear the refire timer
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

void AShooterWeapon::Fire()
{
	// 장전 중이거나, 총알이 없거나, 발사 중지가 되면 쏘지 않음
	if (bIsReloading || CurrentBullets <= 0 || !bIsFiring)
	{
		return;
	}

	// 🎯 만약 점사 카드를 먹어서 점사 모드가 켜져 있다면?
	if (bBurstFire)
	{
		CurrentBurstFired = 0; // 카운터 초기화
		HandleBurstFire();     // 점사 시작!
	}
	else
	{
		// 🔫 기존 일반 발사 로직 (연사/단발)
		FireProjectile(WeaponOwner->GetWeaponTargetLocation());
		TimeOfLastShot = GetWorld()->GetTimeSeconds();
		MakeNoise(ShotLoudness, PawnOwner, PawnOwner->GetActorLocation(), ShotNoiseRange, ShotNoiseTag);

		if (bFullAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AShooterWeapon::Fire, RefireRate, false);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AShooterWeapon::FireCooldownExpired, RefireRate, false);
		}
	}
}

void AShooterWeapon::FireCooldownExpired()
{
	// notify the owner
	WeaponOwner->OnSemiWeaponRefire();
}

void AShooterWeapon::FireProjectile(const FVector& TargetLocation)
{
	// 🎯 샷건 발사: PelletCount(펠릿 개수)만큼 총알을 반복해서 생성합니다.
	for (int32 i = 0; i < PelletCount; ++i)
	{
		// get the projectile transform (반복문 안에 있어야 총알마다 각각 다른 각도로 퍼집니다!)
		FTransform ProjectileTransform = CalculateProjectileSpawnTransform(TargetLocation);

		// spawn the projectile
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = PawnOwner;

		AShooterProjectile* Projectile = GetWorld()->SpawnActor<AShooterProjectile>(ProjectileClass, ProjectileTransform, SpawnParams);
	}

	// ----------------------------------------------------------------------------------
	// ⚠️ 아래부터는 반복문 바깥에 둡니다. (총알이 여러 갈래로 나가도, 쏘는 행위 자체는 1번이니까요!)
	// ----------------------------------------------------------------------------------

	// play the firing montage
	WeaponOwner->PlayFiringMontage(FiringMontage);

	// add recoil
	WeaponOwner->AddWeaponRecoil(FiringRecoil);

	// 총알 감소 (산탄이 나가더라도 탄약은 1발만 소모)
	--CurrentBullets;

	// 총알이 0 이하로 내려가면 0으로 고정
	if (CurrentBullets <= 0)
	{
		CurrentBullets = 0;
		OnRequestReload();
	}

	// update the weapon HUD
	WeaponOwner->UpdateWeaponHUD(CurrentBullets, MagazineSize);
}

FTransform AShooterWeapon::CalculateProjectileSpawnTransform(const FVector& TargetLocation) const
{
	// find the muzzle location
	const FVector MuzzleLoc = FirstPersonMesh->GetSocketLocation(MuzzleSocketName);

	// calculate the spawn location ahead of the muzzle
	const FVector SpawnLoc = MuzzleLoc + ((TargetLocation - MuzzleLoc).GetSafeNormal() * MuzzleOffset);

	// 1. 발사 시작점에서 과녁(Target)을 향하는 '정확한 중앙 방향'을 구합니다.
	const FVector BaseDirection = (TargetLocation - SpawnLoc).GetSafeNormal();

	// 2. VRandCone을 이용해 중앙 방향을 기준으로 'AimVariance(각도)' 안에서 랜덤한 방향을 뽑아냅니다.
	// (언리얼 수학 함수는 Radian을 쓰기 때문에 DegreesToRadians로 변환해 줍니다)
	const FVector ShootDirection = FMath::VRandCone(BaseDirection, FMath::DegreesToRadians(AimVariance));

	// 3. 최종적으로 퍼진 방향을 회전값(Rotator)으로 변환합니다.
	const FRotator AimRot = ShootDirection.Rotation();

	// return the built transform
	return FTransform(AimRot, SpawnLoc, FVector::OneVector);
}

const TSubclassOf<UAnimInstance>& AShooterWeapon::GetFirstPersonAnimInstanceClass() const
{
	return FirstPersonAnimInstanceClass;
}

const TSubclassOf<UAnimInstance>& AShooterWeapon::GetThirdPersonAnimInstanceClass() const
{
	return ThirdPersonAnimInstanceClass;
}
void AShooterWeapon::HandleBurstFire()
{
	// 쏘는 도중에 장전하거나 총알이 다 떨어지면 즉시 점사 중단
	if (bIsReloading || CurrentBullets <= 0)
	{
		CurrentBurstFired = 0;
		return;
	}

	// 1발 발사!
	FireProjectile(WeaponOwner->GetWeaponTargetLocation());
	TimeOfLastShot = GetWorld()->GetTimeSeconds();
	MakeNoise(ShotLoudness, PawnOwner, PawnOwner->GetActorLocation(), ShotNoiseRange, ShotNoiseTag);

	CurrentBurstFired++; // 쏜 횟수 1 증가

	// 아직 목표치(BurstCount)만큼 덜 쐈다면?
	if (CurrentBurstFired < BurstCount)
	{
		// BurstInterval(예: 0.1초) 뒤에 이 함수를 다시 실행해서 또 쏨
		GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &AShooterWeapon::HandleBurstFire, BurstInterval, false);
	}
	else
	{
		// 다 쐈다면 카운터 초기화 (1세트 종료)
		CurrentBurstFired = 0;

		// 점사가 끝나고 다음 클릭(또는 다음 점사)까지 RefireRate(쿨다운)만큼 대기
		if (bFullAuto && bIsFiring)
		{
			// (점사 + 연사 혼합) 마우스를 꾹 누르고 있으면 다다당! 쉬고 다다당! 나감
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AShooterWeapon::Fire, RefireRate, false);
		}
		else
		{
			// (일반 점사) 클릭 한 번에 다다당! 하고 끝남
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AShooterWeapon::FireCooldownExpired, RefireRate, false);
		}
	}
}
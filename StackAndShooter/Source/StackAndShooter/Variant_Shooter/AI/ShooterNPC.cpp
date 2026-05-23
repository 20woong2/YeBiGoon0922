// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/AI/ShooterNPC.h"
#include "ShooterCharacter.h" // 추가: 플레이어 스탯을 읽어오기 위해 포함
#include "Kismet/GameplayStatics.h" // 추가: GetPlayerCharacter를 쓰기 위해 포함
#include "ShooterWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "ShooterGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

void AShooterNPC::BeginPlay()
{
	Super::BeginPlay();

	// spawn the weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Weapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, GetActorTransform(), SpawnParams);
	CurrentHP = MaxHP;
}

void AShooterNPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the death timer
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

float AShooterNPC::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	// ignore if already dead
	if (bIsDead)
	{
		return 0.0f;
	}

	// ========================================================
	// [핵심 추가] 데미지 뻥튀기 로직
	// 1. 현재 맵의 플레이어 캐릭터를 가져옵니다.
	if (AShooterCharacter* PlayerChar = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		// 2. 총알의 기본 데미지에 플레이어의 데미지 배수(예: 1.5)를 곱해줍니다!
		Damage = Damage * PlayerChar->DamageMultiplier;
	}
	// ========================================================

	// Reduce HP (이제 뻥튀기된 데미지로 체력이 깎입니다!)
	CurrentHP -= Damage;

	// Have we depleted HP?
	if (CurrentHP <= 0.0f)
	{
		Die();
	}
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void AShooterNPC::AttachWeaponMeshes(AShooterWeapon* WeaponToAttach)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// attach the weapon actor
	WeaponToAttach->AttachToActor(this, AttachmentRule);

	// attach the weapon meshes
	WeaponToAttach->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	WeaponToAttach->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
}

void AShooterNPC::PlayFiringMontage(UAnimMontage* Montage)
{
	// unused
}

void AShooterNPC::AddWeaponRecoil(float Recoil)
{
	// unused
}

void AShooterNPC::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	// unused
}

FVector AShooterNPC::GetWeaponTargetLocation()
{
	// start aiming from the camera location
	const FVector AimSource = GetFirstPersonCameraComponent()->GetComponentLocation();

	FVector AimDir, AimTarget = FVector::ZeroVector;

	// do we have an aim target?
	if (CurrentAimTarget)
	{
		// target the actor location
		AimTarget = CurrentAimTarget->GetActorLocation();

		// apply a vertical offset to target head/feet
		AimTarget.Z += FMath::RandRange(MinAimOffsetZ, MaxAimOffsetZ);

		// get the aim direction and apply randomness in a cone
		AimDir = (AimTarget - AimSource).GetSafeNormal();
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(AimDir, AimVarianceHalfAngle);

		
	} else {

		// no aim target, so just use the camera facing
		AimDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetFirstPersonCameraComponent()->GetForwardVector(), AimVarianceHalfAngle);

	}

	// calculate the unobstructed aim target location
	AimTarget = AimSource + (AimDir * AimRange);

	// run a visibility trace to see if there's obstructions
	FHitResult OutHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, AimSource, AimTarget, ECC_Visibility, QueryParams);

	// return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AShooterNPC::AddWeaponClass(const TSubclassOf<AShooterWeapon>& InWeaponClass)
{
	// unused
}

void AShooterNPC::OnWeaponActivated(AShooterWeapon* InWeapon)
{
	// unused
}

void AShooterNPC::OnWeaponDeactivated(AShooterWeapon* InWeapon)
{
	// unused
}

void AShooterNPC::OnSemiWeaponRefire()
{
	// are we still shooting?
	if (bIsShooting)
	{
		// fire the weapon
		Weapon->StartFiring();
	}
}

void AShooterNPC::Die()
{
	// ignore if already dead
	if (bIsDead)
	{
		return;
	}

	// raise the dead flag
	bIsDead = true;

	// increment the team score
	if (AShooterGameMode* GM = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncrementTeamScore(TeamByte);
	}

	// disable capsule collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// stop movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->StopActiveMovement();

	// enable ragdoll physics on the third person mesh
	GetMesh()->SetCollisionProfileName(RagdollCollisionProfile);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetPhysicsBlendWeight(1.0f);

	// schedule actor destruction
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AShooterNPC::DeferredDestruction, DeferredDestructionTime, false);
}

void AShooterNPC::DeferredDestruction()
{
	Destroy();
}

void AShooterNPC::StartShooting(AActor* ActorToShoot)
{
	// save the aim target
	CurrentAimTarget = ActorToShoot;

	// raise the flag
	bIsShooting = true;

	// signal the weapon
	Weapon->StartFiring();
}

void AShooterNPC::StopShooting()
{
	// lower the flag
	bIsShooting = false;

	// signal the weapon
	Weapon->StopFiring();
}

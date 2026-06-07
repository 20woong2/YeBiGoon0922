// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterCharacter.h"
#include "ShooterWeapon.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "ShooterGameMode.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AShooterCharacter::AShooterCharacter()
{
	// create the noise emitter component
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Pawn Noise Emitter"));

	// configure movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 게임 시작 시 무기 지급 함수 호출
	GrantDefaultWeapon();
	// reset HP to max
	CurrentHP = MaxHP;

	// update the HUD
	OnDamaged.Broadcast(1.0f);

	// 게임 시작 시 내가 서 있던 자리 기억해두기
	InitialSpawnLoc = GetActorLocation();
}
void AShooterCharacter::GrantDefaultWeapon()
{
	// 우리가 에디터에서 고를 그 '총의 종류'가 설정되어 있다면
	if (DefaultWeaponClass)
	{
		// 그냥 그 '종류(클래스)'만 함수에 쏙 넣어줍니다.
		// 그러면 시스템이 알아서 총을 만들고 손에 쥐여줍니다.
		this->AddWeaponClass(DefaultWeaponClass);
	}
}
void AShooterCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// base class handles move, aim and jump inputs
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AShooterCharacter::DoStartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AShooterCharacter::DoStopFiring);

		// Switch weapon
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoSwitchWeapon);
	}

}

float AShooterCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 1. 죽었는지 확인
	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	// === [여기에 이 한 줄을 꼭 추가해 주세요!] ===
	// 1-2. 무적 상태(CanBeDamaged가 false)인지 확인
	if (!CanBeDamaged())
	{
		return 0.0f;
	}
	// ===========================================

	// 2. 데미지 뻥튀기 계산
	if (EventInstigator && EventInstigator->GetPawn())
	{
		if (AShooterCharacter* AttackerChar = Cast<AShooterCharacter>(EventInstigator->GetPawn()))
		{
			Damage = Damage * AttackerChar->DamageMultiplier;
		}
	}

	// 4. 엔진이 확정한 최종 데미지로 내 체력 깎기
	CurrentHP -= Damage;
	// 3. Super 호출 (수정된 Damage를 엔진에 넘겨서 제대로 소문내기!)
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	

	OnDamaged.Broadcast(FMath::Max(0.0f, CurrentHP / MaxHP));
	// 5. 사망 판정
	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	return Damage;
}

void AShooterCharacter::DoStartFiring()
{
	// fire the current weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFiring();
	}
}

void AShooterCharacter::DoStopFiring()
{
	// stop firing the current weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}
}

void AShooterCharacter::DoSwitchWeapon()
{
	// ensure we have at least two weapons two switch between
	if (OwnedWeapons.Num() > 1)
	{
		// deactivate the old weapon
		CurrentWeapon->DeactivateWeapon();

		// find the index of the current weapon in the owned list
		int32 WeaponIndex = OwnedWeapons.Find(CurrentWeapon);

		// is this the last weapon?
		if (WeaponIndex == OwnedWeapons.Num() - 1)
		{
			// loop back to the beginning of the array
			WeaponIndex = 0;
		}
		else {
			// select the next weapon index
			++WeaponIndex;
		}

		// set the new weapon as current
		CurrentWeapon = OwnedWeapons[WeaponIndex];

		// activate the new weapon
		CurrentWeapon->ActivateWeapon();
	}
}

void AShooterCharacter::AttachWeaponMeshes(AShooterWeapon* Weapon)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// attach the weapon actor
	Weapon->AttachToActor(this, AttachmentRule);

	// attach the weapon meshes
	Weapon->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
	
}

void AShooterCharacter::PlayFiringMontage(UAnimMontage* Montage)
{
	// 1. 내 눈에 보이는 1인칭 팔 메쉬에서 총기 발사 몽타주 재생
	if (GetFirstPersonMesh() && GetFirstPersonMesh()->GetAnimInstance() && Montage)
	{
		GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(Montage);
	}

	// 2. 분할 화면이나 남의 눈에 보이는 3인칭 전신 메쉬에서도 동시에 몽타주 재생
	if (GetMesh() && GetMesh()->GetAnimInstance() && Montage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	}
}

void AShooterCharacter::AddWeaponRecoil(float Recoil)
{
	// apply the recoil as pitch input
	AddControllerPitchInput(Recoil);
}

void AShooterCharacter::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	OnBulletCountUpdated.Broadcast(MagazineSize, CurrentAmmo);
}

FVector AShooterCharacter::GetWeaponTargetLocation()
{
	// trace ahead from the camera viewpoint
	FHitResult OutHit;

	const FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector End = Start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	// return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AShooterCharacter::AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass)
{
	// do we already own this weapon?
	AShooterWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);

	if (!OwnedWeapon)
	{
		// spawn the new weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

		AShooterWeapon* AddedWeapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, GetActorTransform(), SpawnParams);

		if (AddedWeapon)
		{
			// add the weapon to the owned list
			OwnedWeapons.Add(AddedWeapon);

			// if we have an existing weapon, deactivate it
			if (CurrentWeapon)
			{
				CurrentWeapon->DeactivateWeapon();
			}

			// switch to the new weapon
			CurrentWeapon = AddedWeapon;
			CurrentWeapon->ActivateWeapon();
		}
	}
}

void AShooterCharacter::OnWeaponActivated(AShooterWeapon* Weapon)
{
	// update the bullet counter
	OnBulletCountUpdated.Broadcast(Weapon->GetMagazineSize(), Weapon->GetBulletCount());

	// set the character mesh AnimInstances
	GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
}

void AShooterCharacter::OnWeaponDeactivated(AShooterWeapon* Weapon)
{
	// unused
}

void AShooterCharacter::OnSemiWeaponRefire()
{
	// unused
}

AShooterWeapon* AShooterCharacter::FindWeaponOfType(TSubclassOf<AShooterWeapon> WeaponClass) const
{
	// check each owned weapon
	for (AShooterWeapon* Weapon : OwnedWeapons)
	{
		if (Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	// weapon not found
	return nullptr;

}

void AShooterCharacter::Die()
{
	// 1. 무기 비활성화 및 화면에서 숨기기 (허공에 총이 떠있는 버그 방지)
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->DeactivateWeapon();
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	// 2. 캡슐의 움직임을 멈추고 허공에 고정시킵니다.
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	if (GetMesh())
	{
		// 3. 애니메이션 강제 종료
		GetMesh()->bPauseAnims = true;

		// 4. [가장 중요] 캐릭터 몸에 붙은 모든 파츠(옷, 신발 등)를 찾아냅니다.
		TArray<USkeletalMeshComponent*> SkeletalMeshes;
		GetComponents<USkeletalMeshComponent>(SkeletalMeshes);

		for (USkeletalMeshComponent* SkelMesh : SkeletalMeshes)
		{
			if (!SkelMesh) continue;

			// 1인칭 팔(보통 화면에만 보임)은 데스 카메라를 가리지 않게 투명하게 숨깁니다.
			if (SkelMesh == GetFirstPersonMesh())
			{
				SkelMesh->SetHiddenInGame(true);
				continue;
			}

			// 상의, 하의 파츠들을 캡슐에서 떼어내 '추락할 메인 몸통(GetMesh)' 산하로 단단히 묶습니다.
			if (SkelMesh != GetMesh())
			{
				SkelMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
			}
		}

		// 5. 파츠들이 모두 결합된 메인 몸통을 캡슐에서 분리하고 물리(랙돌)를 켭니다.
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
	}

	// 컨트롤 비활성화 및 UI 리셋
	DisableInput(nullptr);
	OnBulletCountUpdated.Broadcast(0, 0);

	// 블루프린트 이벤트 호출
	BP_OnDeath();

	// 리스폰 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AShooterCharacter::OnRespawn, RespawnTime, false);
}

void AShooterCharacter::OnRespawn()
{
	BP_OnRespawn();
	// 1. 입력과 움직임 활성화
	EnableInput(Cast<APlayerController>(GetController()));
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 2. 체력 복구
	ResetHP();

	// === [부활 시 몸통 및 카메라 재조립] ===
	if (GetMesh())
	{
		// 물리 끄고 애니메이션 다시 켜기
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		GetMesh()->bPauseAnims = false;

		// 몸통을 다시 투명 캡슐(루트)에 붙이기
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		// 원래 각도와 위치로 리셋 (유저님 세팅값)
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -70.0f, 0.0f));
	}

	// 숨겼던 1인칭 팔 원상복구
	if (GetFirstPersonMesh())
	{
		GetFirstPersonMesh()->SetHiddenInGame(false);
	}

	// 3. 맵에 있는 PlayerStart(시작 지점)를 찾아서 그곳으로 텔레포트 시킵니다.
	APlayerController* PC = Cast<APlayerController>(GetController());

	// 이 캐릭터가 1P인지 2P인지 번호를 확인합니다. (1P는 0, 2P는 1)
	int32 PlayerIndex = UGameplayStatics::GetPlayerControllerID(PC);

	// 1P면 "P1", 2P면 "P2" 라는 명찰을 타겟으로 정합니다.
	FName TargetTag = (PlayerIndex == 0) ? FName("P1") : FName("P2");

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	bool bFoundSpot = false;

	for (AActor* StartSpot : PlayerStarts)
	{
		// 스폰 지점의 명찰(Tag)이 내 타겟 명찰과 똑같은지 확인합니다!
		if (StartSpot->ActorHasTag(TargetTag))
		{
			FVector SafeLocation = StartSpot->GetActorLocation();
			SafeLocation.Z += 100.0f; // 바닥 끼임 방지를 위해 살짝 띄움

			SetActorLocation(SafeLocation);
			bFoundSpot = true;
			break; // 내 자리를 찾았으니 탐색을 즉시 종료합니다!
		}
	}

	// 만약 실수로 맵에 명찰을 안 달아뒀을 경우를 대비한 안전장치
	if (!bFoundSpot)
	{
		FVector FallbackLocation = GetActorLocation();
		FallbackLocation.Z += 1000.0f;
		SetActorLocation(FallbackLocation);
	}
	
	if (GetMesh())
	{
		// 원래 기본값인 -90도에서 유저님이 돌리셨던 최종 각도 수치(예: -110.0f)를 아래에 적어줍니다.
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -70.0f, 0.0f));
	}

	// 5. 숨겨뒀던 무기를 다시 꺼내서 장착합니다!
	if (IsValid(CurrentWeapon))
	{
		// Die()에 있던 Deactivate의 반대 함수를 실행합니다.
		CurrentWeapon->ActivateWeapon();
	}
	
}

void AShooterCharacter::Heal(float HealAmount)
{
	// 죽은 상태면 회복 불가
	if (CurrentHP <= 0.0f) return;

	// 최대 체력을 넘지 않도록 제한
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0.0f, MaxHP);

	// 체력바 UI 업데이트
	OnDamaged.Broadcast(FMath::Max(0.0f, CurrentHP / MaxHP));
}

void AShooterCharacter::ResetHP()
{
	// 1. 현재 체력을 최대 체력(MaxHP)으로 덮어씌웁니다.
	CurrentHP = MaxHP;

	// 2. 꽉 찬 체력을 체력바 UI에도 알려줍니다! (1.0f는 100% 비율을 의미합니다)
	OnDamaged.Broadcast(1.0f);
}

void AShooterCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	// 원래 있던 Super::FellOutOfWorld(dmgType); 를 절대 적지 않습니다. (엔진의 즉사 기능을 여기서 끊어버립니다)

	//100 데미지 입히기
	TakeDamage(50.0f, FDamageEvent(), nullptr, nullptr);

	// 만약 데미지를 받고도 피가 남아서 살아있다면
	if (CurrentHP > 0.0f)
	{
		// 기억해둔 시작 위치로 텔레포트
		// 3. 맵에 있는 PlayerStart(시작 지점)를 찾아서 그곳으로 텔레포트 시킵니다.
		APlayerController* PC = Cast<APlayerController>(GetController());

		// 이 캐릭터가 1P인지 2P인지 번호를 확인합니다. (1P는 0, 2P는 1)
		int32 PlayerIndex = UGameplayStatics::GetPlayerControllerID(PC);

		// 1P면 "P1", 2P면 "P2" 라는 명찰을 타겟으로 정합니다.
		FName TargetTag = (PlayerIndex == 0) ? FName("P1") : FName("P2");

		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

		bool bFoundSpot = false;

		for (AActor* StartSpot : PlayerStarts)
		{
			// 스폰 지점의 명찰(Tag)이 내 타겟 명찰과 똑같은지 확인합니다!
			if (StartSpot->ActorHasTag(TargetTag))
			{
				FVector SafeLocation = StartSpot->GetActorLocation();
				SafeLocation.Z += 100.0f; // 바닥 끼임 방지를 위해 살짝 띄움

				SetActorLocation(SafeLocation);
				bFoundSpot = true;
				break; // 내 자리를 찾았으니 탐색을 즉시 종료합니다!
			}
		}

		// 만약 실수로 맵에 명찰을 안 달아뒀을 경우를 대비한 안전장치
		if (!bFoundSpot)
		{
			FVector FallbackLocation = GetActorLocation();
			FallbackLocation.Z += 1000.0f;
			SetActorLocation(FallbackLocation);
		}
	}
}

void AShooterCharacter::ForceUpdateWeaponHUD()
{
	// 현재 무기를 안전하게 들고 있다면, 그 무기의 맥스 탄창과 현재 총알을 UI에 방송합니다!
	if (IsValid(CurrentWeapon))
	{
		OnBulletCountUpdated.Broadcast(CurrentWeapon->GetMagazineSize(), CurrentWeapon->GetBulletCount());
	}
}

void AShooterCharacter::AddControllerYawInput(float Val)
{
	// 블루프린트에서 계산한 최종 감도 배율을 가져옵니다.
	float Multiplier = GetCustomLookMultiplier();

	// 블루프린트에 구현이 안 되어 있거나 실수로 0이 반환되면 멈추는 것을 방지하는 안전장치
	if (Multiplier <= 0.0f)
	{
		Multiplier = 1.0f;
	}

	// 입력값(Val)에 최종 감도를 곱해서 진짜 회전 로직(Super)으로 넘겨줍니다!
	Super::AddControllerYawInput(Val * Multiplier);
}

void AShooterCharacter::AddControllerPitchInput(float Val)
{
	float Multiplier = GetCustomLookMultiplier();
	if (Multiplier <= 0.0f)
	{
		Multiplier = 1.0f;
	}

	Super::AddControllerPitchInput(Val * Multiplier);
}
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
	// ignore if already dead
	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	// Reduce HP
	CurrentHP -= Damage;

	// Have we depleted HP?
	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	// update the HUD
	OnDamaged.Broadcast(FMath::Max(0.0f, CurrentHP / MaxHP));
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
	// deactivate the weapon
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->DeactivateWeapon();
	}

	// increment the team score
	if (AShooterGameMode* GM = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncrementTeamScore(TeamByte);
	}
		
	// stop character movement
	GetCharacterMovement()->StopMovementImmediately();

	// disable controls
	DisableInput(nullptr);

	// reset the bullet counter UI
	OnBulletCountUpdated.Broadcast(0, 0);

	// call the BP handler
	BP_OnDeath();

	// schedule character respawn
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AShooterCharacter::OnRespawn, RespawnTime, false);
}

void AShooterCharacter::OnRespawn()
{
	BP_OnRespawn();
	// 1. 입력과 움직임을 다시 활성화합니다. (Die에서 막았던 것 복구)
	EnableInput(Cast<APlayerController>(GetController()));
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 2. 체력(Health)을 다시 최대치로 채워줍니다.
	// (🚨주의: 유저님의 실제 체력 변수명과 최대 체력 변수명으로 수정해 주세요!)
	ResetHP();

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
	TakeDamage(100.0f, FDamageEvent(), nullptr, nullptr);

	// 만약 데미지를 받고도 피가 남아서 살아있다면
	if (CurrentHP > 0.0f)
	{
		// 기억해둔 시작 위치로 텔레포트
		SetActorLocation(InitialSpawnLoc);
	}
}
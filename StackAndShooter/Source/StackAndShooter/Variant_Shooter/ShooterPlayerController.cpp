// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/ShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "ShooterCharacter.h"
#include "ShooterBulletCounterUI.h"
#include "StackAndShooter.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
}

void AShooterPlayerController::SetupInputComponent()
{
	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// add the input mapping contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void AShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// only spawn touch controls on local player controllers
	if (IsLocalPlayerController())
	{
		if (SVirtualJoystick::ShouldDisplayTouchInterface())
		{
			// spawn the mobile controls widget
			MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

			if (MobileControlsWidget)
			{
				// add the controls to the player screen
				MobileControlsWidget->AddToPlayerScreen(0);

			}
			else {

				UE_LOG(LogStackAndShooter, Error, TEXT("Could not spawn mobile controls widget."));

			}
		}

		// create the bullet counter widget and add it to the screen
		BulletCounterUI = CreateWidget<UShooterBulletCounterUI>(this, BulletCounterUIClass);

		if (BulletCounterUI)
		{
			BulletCounterUI->AddToPlayerScreen(0);

		}
		else {

			UE_LOG(LogStackAndShooter, Error, TEXT("Could not spawn bullet counter widget."));

		}

	}

	if (AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(InPawn))
	{
		ShooterCharacter->Tags.Add(PlayerPawnTag);
		ShooterCharacter->OnBulletCountUpdated.AddDynamic(this, &AShooterPlayerController::OnBulletCountUpdated);
		ShooterCharacter->OnDamaged.AddDynamic(this, &AShooterPlayerController::OnPawnDamaged);

		// 1. 체력바 강제 업데이트 (기존 코드)
		ShooterCharacter->OnDamaged.Broadcast(1.0f);

		// 🚀 2. 여기에 추가: 방금 만든 총알 UI 강제 업데이트 함수 호출!
		ShooterCharacter->ForceUpdateWeaponHUD();
	}


	// subscribe to the pawn's OnDestroyed delegate
	InPawn->OnDestroyed.AddDynamic(this, &AShooterPlayerController::OnPawnDestroyed);

	// is this a shooter character?
	if (AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(InPawn))
	{
		// add the player tag
		ShooterCharacter->Tags.Add(PlayerPawnTag);

		// subscribe to the pawn's delegates
		ShooterCharacter->OnBulletCountUpdated.AddDynamic(this, &AShooterPlayerController::OnBulletCountUpdated);
		ShooterCharacter->OnDamaged.AddDynamic(this, &AShooterPlayerController::OnPawnDamaged);

		// force update the life bar
		ShooterCharacter->OnDamaged.Broadcast(1.0f);
	}
}

void AShooterPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
	// reset the bullet counter HUD
	if (IsValid(BulletCounterUI))
	{
		BulletCounterUI->BP_UpdateBulletCounter(0, 0);
	}

	// find the player start
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		// select a random player start
		AActor* RandomPlayerStart = ActorList[FMath::RandRange(0, ActorList.Num() - 1)];

		// spawn a character at the player start
		const FTransform SpawnTransform = RandomPlayerStart->GetActorTransform();

		if (AShooterCharacter* RespawnedCharacter = GetWorld()->SpawnActor<AShooterCharacter>(CharacterClass, SpawnTransform))
		{
			// possess the character
			Possess(RespawnedCharacter);
		}
	}
}

void AShooterPlayerController::OnBulletCountUpdated(int32 MagazineSize, int32 Bullets)
{
	// update the UI
	if (BulletCounterUI)
	{
		BulletCounterUI->BP_UpdateBulletCounter(MagazineSize, Bullets);
	}
}

void AShooterPlayerController::OnPawnDamaged(float LifePercent)
{
	if (IsValid(BulletCounterUI))
	{
		BulletCounterUI->BP_Damaged(LifePercent);
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.


#include "StackAndShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "StackAndShooterCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "StackAndShooter.h"
#include "Widgets/Input/SVirtualJoystick.h"

AStackAndShooterPlayerController::AStackAndShooterPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AStackAndShooterCameraManager::StaticClass();
}

void AStackAndShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// [수정] 게임 시작하자마자 바로 만들지 말고, 0.2초 뒤에 SpawnMobileControls 함수를 실행해라!
	GetWorld()->GetTimerManager().SetTimer(MobileControlsTimerHandle, this, &AStackAndShooterPlayerController::SpawnMobileControls, 0.2f, false);
}
void AStackAndShooterPlayerController::SpawnMobileControls()
{
	// GetLocalPlayer()가 완벽하게 붙었는지(null이 아닌지) 한 번 더 체크하는 철벽 안전장치!
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController() && GetLocalPlayer() != nullptr)
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);
		}
		else
		{
			UE_LOG(LogStackAndShooter, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}
}

void AStackAndShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
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

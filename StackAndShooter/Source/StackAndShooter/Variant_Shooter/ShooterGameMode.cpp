// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/ShooterGameMode.h"
#include "ShooterUI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// create the UI
	ShooterUI = CreateWidget<UShooterUI>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ShooterUIClass);
	ShooterUI->AddToViewport(0);
}

void AShooterGameMode::IncrementTeamScore(uint8 TeamByte)
{
	// 🚨 거짓말 탐지기 가동! 도대체 어떤 놈이 날 부르고 있는가?!
	UE_LOG(LogTemp, Warning, TEXT("====================================="));
	UE_LOG(LogTemp, Warning, TEXT("[점수 함수 호출됨!] 방금 죽었다고 신고한 TeamByte 번호: %d"), TeamByte);

		// 죽은 팀이 0번이면 승자는 1번, 죽은 팀이 1번이면 승자는 0번
		uint8 WinnerTeam = (TeamByte == 0) ? 1 : 0;

		int32 Score = 0;
		if (int32* FoundScore = TeamScores.Find(WinnerTeam)) // WinnerTeam의 점수를 찾
		{
			Score = *FoundScore;
		}

		
		++Score;
		TeamScores.Add(WinnerTeam, Score); // WinnerTeam에게 점수

		// update the UI
		ShooterUI->BP_UpdateScore(WinnerTeam, Score);

		// 블루프린트에게 전달
		OnScoreIncremented(WinnerTeam, Score);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CrustyPirateGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UCrustyPirateGameInstance::SetPlayerHP(int32 NewHP)
{
	PlayerHP = NewHP;
}

void UCrustyPirateGameInstance::AddPlayerDiamonds(int32 Amount)
{
	DiamondsCollected += Amount;
}

void UCrustyPirateGameInstance::SetDoubleJumpUnlocked(bool bUnlocked)
{
	bIsDoubleJumpUnlocked = bUnlocked;
}

void UCrustyPirateGameInstance::ChangeLevel(int32 LevelIndex)
{
	if (LevelIndex <= 0)
		return;

	CurrentLevelIndex = LevelIndex;
	FString levelStr = FString::Printf(TEXT("Level_%d"), LevelIndex);
	UGameplayStatics::OpenLevel(GetWorld(), FName(levelStr));
}

void UCrustyPirateGameInstance::RestartGame()
{
	PlayerHP = DefaultPlayerHP;
	DiamondsCollected = 0;
	bIsDoubleJumpUnlocked = false;

	ChangeLevel(1);
}

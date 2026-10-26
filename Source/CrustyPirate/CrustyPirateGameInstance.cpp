// Fill out your copyright notice in the Description page of Project Settings.


#include "CrustyPirateGameInstance.h"

void UCrustyPirateGameInstance::SetPlayerHP(int NewHP)
{
	PlayerHP = NewHP;
}

void UCrustyPirateGameInstance::AddPlayerDiamonds(int NewDiamonds)
{
	DiamondsCollected += NewDiamonds;
}

void UCrustyPirateGameInstance::ChangeLevel(int LevelIndex)
{
	if (LevelIndex <= 0)
		return;

	CurrentlLevelIndex = LevelIndex;
	FString levelStr = FString::Printf(TEXT("Level_%d"), LevelIndex);
	UGameplayStatics::OpenLevel(GetWorld(), FName(levelStr));
}

void UCrustyPirateGameInstance::RestartGame()
{
	PlayerHP = 100;
	DiamondsCollected = 0;
	bIsDoubleJumpUnlocked = false;

	CurrentlLevelIndex = 1;
	ChangeLevel(CurrentlLevelIndex);
}
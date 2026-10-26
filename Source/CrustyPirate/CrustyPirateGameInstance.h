// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CrustyPirateGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UCrustyPirateGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PlayerHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int DiamondsCollected = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDoubleJumpUnlocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentlLevelIndex = 1;

	void SetPlayerHP(int NewHP);
	void AddPlayerDiamonds(int NewDiamonds);
	void ChangeLevel(int LevelIndex);
	void RestartGame();
};

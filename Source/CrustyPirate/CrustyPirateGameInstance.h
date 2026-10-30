// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrustyPirateGameInstance.generated.h"

/**
 *
 */
UCLASS()
class CRUSTYPIRATE_API UCrustyPirateGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	int32 GetPlayerHP() const { return PlayerHP; }
	int32 GetDiamondsCollected() const { return DiamondsCollected; }
	bool IsDoubleJumpUnlocked() const { return bIsDoubleJumpUnlocked; }
	int32 GetCurrentLevelIndex() const { return CurrentLevelIndex; }

	void SetPlayerHP(int32 NewHP);
	void AddPlayerDiamonds(int32 Amount);
	void SetDoubleJumpUnlocked(bool bUnlocked);
	void ChangeLevel(int32 LevelIndex);
	void RestartGame();

private:
	static constexpr int32 DefaultPlayerHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 PlayerHP = DefaultPlayerHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 DiamondsCollected = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsDoubleJumpUnlocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevelIndex = 1;
};

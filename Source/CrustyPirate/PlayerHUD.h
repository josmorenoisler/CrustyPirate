// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class CRUSTYPIRATE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHP(int32 NewHP);
	void SetDiamonds(int32 NewDiamonds);
	void SetLevel(int32 NewLevel);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DiamondsText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;
};

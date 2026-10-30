// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/TextBlock.h"

void UPlayerHUD::SetHP(int32 NewHP)
{
	FString strHP = FString::Printf(TEXT("HP: %d"), NewHP);
	HPText->SetText(FText::FromString(strHP));
}

void UPlayerHUD::SetDiamonds(int32 NewDiamonds)
{
	FString strDiamonds = FString::Printf(TEXT("Diamonds: %d"), NewDiamonds);
	DiamondsText->SetText(FText::FromString(strDiamonds));
}

void UPlayerHUD::SetLevel(int32 NewLevel)
{
	FString strLevel = FString::Printf(TEXT("Level - %d"), NewLevel);
	LevelText->SetText(FText::FromString(strLevel));
}

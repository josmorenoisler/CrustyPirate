// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/TimerHandle.h"
#include "ExitLevelClass.generated.h"

UCLASS()
class CRUSTYPIRATE_API AExitLevelClass : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* DoorFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ExitDoorSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NextLevelIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeInSecs = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsActive = true;

	FTimerHandle WaitTimer;

	AExitLevelClass();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnWaitTimerTimeout();
};

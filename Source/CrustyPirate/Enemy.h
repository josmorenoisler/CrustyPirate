// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "Components/TextRenderComponent.h"
#include "PaperZDAnimInstance.h"
#include "Engine/TimerHandle.h"
#include "Components/BoxComponent.h"

#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* PlayerDetectionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextRenderComponent* HPText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayerCharacter* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AttackAnimSeq;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* AttackCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistToTarget = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCDInSecs = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitPoints = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackStunDuration = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanAttack = true;

	FTimerHandle StunTimer;
	FTimerHandle AttackCDTimer;

	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;

	AEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool ShouldMoveToTarget();
	void UpdateDirectionEnemy(float MoveDirection);
	void UpdateHP(int NewHp);
	void TakeDamage(int DamageAmount, float StunDuration);
	void Stun(float DurationInSeconds);
	void OnStunTimerTimeout();

	void Attack();
	void OnAttackCDTimerTimeout();
	void OnAttackOverrideAnimEnd(bool bCompleted);

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool bEnabled);
};

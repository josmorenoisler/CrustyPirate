// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Engine/TimerHandle.h"
#include "Components/BoxComponent.h"
#include "PlayerHUD.h"
#include "CrustyPirateGameInstance.h"
#include "Sound/SoundBase.h"
#include "CollectableItem.h"

#include "PaperZDAnimInstance.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API APlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* AttackCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* QuitAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ItemPickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AttackAnimSeq;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerHUD* PlayerHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCrustyPirateGameInstance* MyGameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsActive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDamage = 25;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int HitPoints = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackStunDuration = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsStunned = false;

	FTimerHandle PlayerStunTimer;
	FTimerHandle RestartTimer;
	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;

	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& value);
	void JumpStart(const FInputActionValue& value);
	void JumpEnd(const FInputActionValue& value);
	void Attack(const FInputActionValue& value);
	void UpdateHP(int NewHp);
	void TakeDamage(int DamageAmount, float StunDuration);
	void Stun(float DurationInSeconds);
	void OnStunTimerTimeout();
	void CollectItem(CollectableType ItemType);
	void UnlockDoubleJump();

	void UpdateDirection(float MoveDirection);
	void OnAttackAnimationEnd(bool bCompleted);
	void OnRestartTimerTiemout();
	void DeactivatePlayer();
	void QuitGame();

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool bEnabled);
};

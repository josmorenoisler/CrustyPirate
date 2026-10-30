// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Enemy.h"
#include "PlayerHUD.h"
#include "CrustyPirateGameInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	// Nothing runs in Tick yet, so don't pay for it every frame. Turn this back on if Tick gets logic.
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input mapping context
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	OnAttackOverrideEndDelegate.BindUObject(this, &APlayerCharacter::OnAttackAnimationEnd);
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);

	MyGameInstance = Cast<UCrustyPirateGameInstance>(GetGameInstance());
	if (MyGameInstance)
	{
		HitPoints = MyGameInstance->GetPlayerHP();
		if (MyGameInstance->IsDoubleJumpUnlocked())
		{
			UnlockDoubleJump();
		}
	}

	if (PlayerHUDClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PlayerHUDClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToPlayerScreen();
			PlayerHUDWidget->SetHP(HitPoints);
			if (MyGameInstance)
			{
				PlayerHUDWidget->SetDiamonds(MyGameInstance->GetDiamondsCollected());
				PlayerHUDWidget->SetLevel(MyGameInstance->GetCurrentLevelIndex());
			}
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStart);

		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnd);

		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &APlayerCharacter::JumpEnd);

		enhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);

		enhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started, this, &APlayerCharacter::QuitGame);
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (bIsAlive && bCanMove && !bIsStunned)
	{
		const float moveActionValue = value.Get<float>();
		AddMovementInput(FVector::ForwardVector, moveActionValue);
		UpdateDirection(moveActionValue);
	}
}

void APlayerCharacter::JumpStart(const FInputActionValue& value)
{
	if (bIsAlive && bCanMove && !bIsStunned)
	{
		Jump();
	}
}

void APlayerCharacter::JumpEnd(const FInputActionValue& value)
{
	StopJumping();
}

void APlayerCharacter::Attack(const FInputActionValue& value)
{
	if (bIsAlive && bCanAttack && !bIsStunned)
	{
		bCanAttack = false;
		bCanMove = false;

		//EnableAttackCollisionBox(true);

		GetAnimInstance()->PlayAnimationOverride(AttackAnimSeq, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);
	}
}

void APlayerCharacter::UpdateHP(int32 NewHp)
{
	HitPoints = NewHp;

	if (MyGameInstance)
	{
		MyGameInstance->SetPlayerHP(HitPoints);
	}

	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->SetHP(HitPoints);
	}
}

void APlayerCharacter::TakeHit(int32 DamageAmount, float StunDuration)
{
	if (!bIsActive || !bIsAlive) return;

	Stun(StunDuration);
	UpdateHP(HitPoints - DamageAmount);

	if (HitPoints <= 0)
	{
		UpdateHP(0);

		bIsAlive = false;
		bCanMove = false;
		bCanAttack = false;

		GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CaptainStateMachine"));

		EnableAttackCollisionBox(false);

		GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerCharacter::OnRestartTimerTimeout, 1.0f, false, 3.0f); // set restart time to a variable
	}
	else
	{
		GetAnimInstance()->JumpToNode(FName("JumpTakeHit"), FName("CaptainStateMachine"));
	}
}

void APlayerCharacter::OnAttackAnimationEnd(bool bCompleted)
{
	if (bIsAlive && bIsActive)
	{
		bCanAttack = true;
		bCanMove = true;
	}
}

void APlayerCharacter::OnRestartTimerTimeout()
{
	if (MyGameInstance)
	{
		MyGameInstance->RestartGame();
	}
}

void APlayerCharacter::DeactivatePlayer()
{
	if (bIsActive)
	{
		bIsActive = false;
		bCanAttack = false;
		bCanMove = false;

		GetCharacterMovement()->StopMovementImmediately();
	}
}

void APlayerCharacter::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemy* enemy = Cast<AEnemy>(OtherActor))
	{
		enemy->TakeHit(AttackDamage, AttackStunDuration);
	}
}

void APlayerCharacter::EnableAttackCollisionBox(bool bEnabled)
{
	if (bEnabled)
	{
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	else
	{
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

void APlayerCharacter::UpdateDirection(float MoveDirection)
{
	FRotator currentRotation = Controller->GetControlRotation();

	if (MoveDirection < 0.0f)
	{
		if (currentRotation.Yaw != 180.0f)
		{
			Controller->SetControlRotation(FRotator(currentRotation.Pitch, 180.0f, currentRotation.Roll));
		}
	}
	else if (MoveDirection > 0.0f)
	{
		if (currentRotation.Yaw != 0.0f)
		{
			Controller->SetControlRotation(FRotator(currentRotation.Pitch, 0.0f, currentRotation.Roll));
		}
	}
}

void APlayerCharacter::Stun(float DurationInSeconds)
{
	bIsStunned = true;

	// SetTimer on a handle that is already running restarts it, so no need to clear it first
	GetWorldTimerManager().SetTimer(PlayerStunTimer, this, &APlayerCharacter::OnStunTimerTimeout, 1.0f, false, DurationInSeconds);

	GetAnimInstance()->StopAllAnimationOverrides();

	EnableAttackCollisionBox(false);
}

void APlayerCharacter::OnStunTimerTimeout()
{
	bIsStunned = false;
}

void APlayerCharacter::CollectItem(CollectableType ItemType)
{
	UGameplayStatics::PlaySound2D(GetWorld(), ItemPickUpSound);

	switch (ItemType)
	{
	case CollectableType::Diamond:
		if (MyGameInstance)
		{
			MyGameInstance->AddPlayerDiamonds(1);
			if (PlayerHUDWidget)
			{
				PlayerHUDWidget->SetDiamonds(MyGameInstance->GetDiamondsCollected());
			}
		}
		break;
	case CollectableType::HealthPotion:
		UpdateHP(HitPoints + 25); //put 25 in a variable for the future
		break;
	case CollectableType::DoubleJumpUpgrade:
		if (MyGameInstance)
		{
			MyGameInstance->SetDoubleJumpUnlocked(true);
		}
		UnlockDoubleJump();
		break;
	default:
		break;
	}
}

void APlayerCharacter::UnlockDoubleJump()
{
	JumpMaxCount = 2;
}

void APlayerCharacter::QuitGame()
{
	UWorld* world = GetWorld();
	UKismetSystemLibrary::QuitGame(world, UGameplayStatics::GetPlayerController(world, 0), EQuitPreference::Quit, false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectionSphere"));
	PlayerDetectionSphere->SetupAttachment(RootComponent);

	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HPText"));
	HPText->SetupAttachment(RootComponent);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	PlayerDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	PlayerDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);
	UpdateHP(HitPoints);

	OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::OnAttackOverrideAnimEnd);

	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAlive && FollowTarget && !bIsStunned)
	{
		float moveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.0f ? 1.0f : -1.0f;
		if (ShouldMoveToTarget())
		{
			if (bCanMove)
			{
				FVector worldDirection = FVector(1.0f, 0.0f, 0.0f);
				AddMovementInput(worldDirection, moveDirection);
			}
		}
		else
		{
			if (FollowTarget->bIsAlive)
			{
				Attack();
			}
		}
	}
}

void AEnemy::UpdateDirectionEnemy(float MoveDirection)
{
	FRotator currentRotation = GetActorRotation();

	if (MoveDirection < 0.0f)
	{
		if (currentRotation.Yaw != 180.0f)
		{
			SetActorRotation(FRotator(currentRotation.Pitch, 180.0f, currentRotation.Roll));
		}
	}
	else if (MoveDirection > 0.0f)
	{
		if (currentRotation.Yaw != 0.0f)
		{
			SetActorRotation(FRotator(currentRotation.Pitch, 0.0f, currentRotation.Roll));
		}
	}
}

void AEnemy::UpdateHP(int NewHp)
{
	HitPoints = NewHp;

	FString strHP = FString::Printf(TEXT("HP: %d"), HitPoints);
	HPText->SetText(FText::FromString(strHP));
}

void AEnemy::TakeDamage(int DamageAmount, float StunDuration)
{
	if (!bIsAlive) return;
	
	Stun(StunDuration);
	UpdateHP(HitPoints - DamageAmount);

	if (HitPoints <= 0)
	{
		UpdateHP(0);
		HPText->SetHiddenInGame(true);
		bIsAlive = false;
		bCanMove = false;
		bCanAttack = false;

		GetAnimInstance()->JumpToNode(FName("JumpDie"));

		EnableAttackCollisionBox(false);
	}
	else 
	{
		GetAnimInstance()->JumpToNode(FName("JumpTakeHit"));
	}
}

void AEnemy::Stun(float DurationInSeconds)
{
	bIsStunned = true;
	bool bIsTimerActive = GetWorldTimerManager().IsTimerActive(StunTimer);

	if (bIsTimerActive)
		GetWorldTimerManager().ClearTimer(StunTimer);

	GetWorldTimerManager().SetTimer(StunTimer, this, &AEnemy::OnStunTimerTimeout, 1.0f, false, DurationInSeconds);

	GetAnimInstance()->StopAllAnimationOverrides();

	EnableAttackCollisionBox(false);
}

void AEnemy::OnStunTimerTimeout()
{
	bIsStunned = false;
}

void AEnemy::Attack()
{
	if (bIsAlive && bCanAttack && !bIsStunned)
	{
		bCanAttack = false;
		bCanMove = false;

		GetAnimInstance()->PlayAnimationOverride(AttackAnimSeq, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegate);

		GetWorldTimerManager().SetTimer(AttackCDTimer, this, &AEnemy::OnAttackCDTimerTimeout, 1.0f, false, AttackCDInSecs);
	}
}

void AEnemy::OnAttackCDTimerTimeout()
{
	bCanAttack = true;
}

void AEnemy::OnAttackOverrideAnimEnd(bool bCompleted)
{
	if (bIsAlive)
	{
		bCanMove = true;
	}
}

void AEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
		FollowTarget = player;
	}
}

void AEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
		FollowTarget = NULL;
	}
}

bool AEnemy::ShouldMoveToTarget()
{
	bool result = false;

	if (FollowTarget)
	{
		float distToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
		result = distToTarget > StopDistToTarget;
	}

	return result;
}

void AEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player)
	{
		player->TakeDamage(AttackDamage, AttackStunDuration);
	}
}

void AEnemy::EnableAttackCollisionBox(bool bEnabled)
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
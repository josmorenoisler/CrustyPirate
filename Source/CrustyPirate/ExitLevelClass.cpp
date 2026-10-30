// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitLevelClass.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"
#include "PlayerCharacter.h"
#include "CrustyPirateGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExitLevelClass::AExitLevelClass()
{
	// Nothing runs in Tick yet, so don't pay for it every frame. Turn this back on if Tick gets logic.
	PrimaryActorTick.bCanEverTick = false;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
	DoorFlipbook->SetupAttachment(RootComponent);

	DoorFlipbook->SetPlayRate(0.0f);
	DoorFlipbook->SetLooping(false);
}

// Called when the game starts or when spawned
void AExitLevelClass::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AExitLevelClass::OverlapBegin);
	DoorFlipbook->SetPlaybackPosition(0.0f, false);
}

// Called every frame
void AExitLevelClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitLevelClass::OnWaitTimerTimeout()
{
	if (UCrustyPirateGameInstance* myGameInstance = Cast<UCrustyPirateGameInstance>(GetGameInstance()))
	{
		// An exit that points back to an earlier level means this was the last one, so start a fresh run
		if (NextLevelIndex <= myGameInstance->GetCurrentLevelIndex())
		{
			myGameInstance->RestartGame();
		}
		else
		{
			myGameInstance->ChangeLevel(NextLevelIndex);
		}
	}
}

void AExitLevelClass::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive) return;

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player && player->IsAlive())
	{
		player->DeactivatePlayer();
		bIsActive = false;
		DoorFlipbook->SetPlayRate(1.0f);
		DoorFlipbook->PlayFromStart();

		UGameplayStatics::PlaySound2D(GetWorld(), ExitDoorSound);

		GetWorldTimerManager().SetTimer(WaitTimer, this, &AExitLevelClass::OnWaitTimerTimeout, 1.0f, false, WaitTimeInSecs);
	}
}

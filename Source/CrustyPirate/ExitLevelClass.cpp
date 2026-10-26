// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitLevelClass.h"
#include "PlayerCharacter.h"
#include "CrustyPirateGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExitLevelClass::AExitLevelClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	UCrustyPirateGameInstance* myGameInstance = Cast<UCrustyPirateGameInstance>(GetGameInstance());
	if (myGameInstance)
	{
		myGameInstance->ChangeLevel(NextLevelIndex);
	}
}

void AExitLevelClass::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player && player->bIsAlive)
	{
		if (bIsActive)
		{
			player->DeactivatePlayer();
			bIsActive = false;
			DoorFlipbook->SetPlayRate(1.0f);
			DoorFlipbook->PlayFromStart();

			UGameplayStatics::PlaySound2D(GetWorld(), ExitDoorSound);

			GetWorldTimerManager().SetTimer(WaitTimer, this, &AExitLevelClass::OnWaitTimerTimeout, 1.0f, false, WaitTimeInSecs); // set restart time to a variable

		}
	}
}


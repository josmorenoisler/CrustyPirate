// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItem.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "PlayerCharacter.h"

// Sets default values
ACollectableItem::ACollectableItem()
{
	// Nothing runs in Tick yet, so don't pay for it every frame. Turn this back on if Tick gets logic.
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	ItemFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ItemFlipbook"));
	ItemFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACollectableItem::OverlapBegin);
}

// Called every frame
void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableItem::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player && player->IsAlive() && player->IsActive())
	{
		player->CollectItem(CollectableType);
		Destroy();
	}
}

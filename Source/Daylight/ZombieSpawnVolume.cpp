// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawnVolume.h"
#include "ZombieBase.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AZombieSpawnVolume::AZombieSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;

	SpawnBox->SetBoxExtent(FVector(1000.f, 1000.f, 200.f));
	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 비주얼 표시 (에디터에서만)
#if WITH_EDITORONLY_DATA
	SpawnBox->bDrawOnlyIfSelected = true;
	SpawnBox->ShapeColor = FColor::Green;
#endif

}

// Called when the game starts or when spawned
void AZombieSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AZombieSpawnVolume::GetRandomPointInVolume() const
{
	FVector Origin = SpawnBox->Bounds.Origin;
	FVector BoxExtent = SpawnBox->Bounds.BoxExtent;
	
	float RandomX = FMath::RandRange(-BoxExtent.X, BoxExtent.X);
	float RandomY = FMath::RandRange(-BoxExtent.Y, BoxExtent.Y);
	float RandomZ = FMath::RandRange(-BoxExtent.Z, BoxExtent.Z);

	return Origin + FVector(RandomX, RandomY, RandomZ);
}

bool AZombieSpawnVolume::FindValidSpawnLocation(FVector& OutLocation) const
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if(!NavSys)
	{
		return false;
	}

	for (int32 i = 0; i < 10; ++i)
	{
		FVector RandomPoint = GetRandomPointInVolume();
		FNavLocation NavLocation;
		if(NavSys->GetRandomPointInNavigableRadius(RandomPoint, 200.f, NavLocation))
		{
			OutLocation = NavLocation.Location;
			return true;
		}
	}

	return false;
}

AZombieBase* AZombieSpawnVolume::SpawnZombie()
{
	if(!ZombieClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ZombieClass is not set in ZombieSpawnVolume"));
		return nullptr;
	}
	FVector SpawnLocation;
	if(!FindValidSpawnLocation(SpawnLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find valid spawn location"));
		return nullptr;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AZombieBase* SpawnedZombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	if (SpawnedZombie)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned zombie at %s"), *SpawnLocation.ToString());
	}

	return SpawnedZombie;
}

void AZombieSpawnVolume::SpawnZombies(int32 Count)
{
	for (int32 i = 0; i < Count; i++)
	{
		SpawnZombie();
	}

	UE_LOG(LogTemp, Log, TEXT("Spawned %d zombies"), Count);
}
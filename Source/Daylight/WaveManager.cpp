// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "ZombieSpawnVolume.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // 기본 웨이브 설정
    FWaveData Wave1;
    Wave1.ZombieCount = 10;
    Wave1.SpawnDelay = 1.0f;
    Wave1.PrepareTime = 3.0f;
    Waves.Add(Wave1);

    FWaveData Wave2;
    Wave2.ZombieCount = 20;
    Wave2.SpawnDelay = 0.8f;
    Wave2.PrepareTime = 5.0f;
    Waves.Add(Wave2);

    FWaveData Wave3;
    Wave3.ZombieCount = 30;
    Wave3.SpawnDelay = 0.5f;
    Wave3.PrepareTime = 5.0f;
    Waves.Add(Wave3);

}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
    StartWave();
	
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveManager::StartWave()
{
   if(CurrentWave >= Waves.Num())
   {
       UE_LOG(LogTemp, Warning, TEXT("All waves completed!"));
       return;
   }

   if(SpawnVolumes.Num() == 0)
   {
       UE_LOG(LogTemp, Warning, TEXT("No spawn volumes assigned!"));
       return;
   }

   bWaveInProgress = true;
   // start spawning zombies for the current wave
   SpawnedThisWave = 0;
   RemainingZombies = Waves[CurrentWave].ZombieCount;

   UE_LOG(LogTemp, Log, TEXT("Starting Wave %d - %d zombies"), CurrentWave + 1, RemainingZombies);

   // 첫 좀비 스폰 시작
   GetWorldTimerManager().SetTimer(
       SpawnTimerHandle,
       this,
       &AWaveManager::SpawnNextZombie,
       Waves[CurrentWave].SpawnDelay,
       true,
       Waves[CurrentWave].PrepareTime  // 준비 시간 후 시작
   );
}


void AWaveManager::SpawnNextZombie()
{
    if (SpawnedThisWave >= Waves[CurrentWave].ZombieCount)
    {
        // 스폰 완료
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    // 랜덤 스폰 볼륨에서 생성
    AZombieSpawnVolume* SpawnVolume = GetRandomSpawnVolume();
    if (SpawnVolume)
    {
        SpawnVolume->SpawnZombie();
        SpawnedThisWave++;
    }
}

void AWaveManager::OnZombieKilled()
{
    RemainingZombies--;

    UE_LOG(LogTemp, Log, TEXT("Zombie killed. Remaining: %d"), RemainingZombies);

    if (RemainingZombies <= 0 && SpawnedThisWave >= Waves[CurrentWave].ZombieCount)
    {
        OnWaveComplete();
    }
}

void AWaveManager::OnWaveComplete()
{
    bWaveInProgress = false;

    UE_LOG(LogTemp, Warning, TEXT("Wave %d completed!"), CurrentWave + 1);

    if (CurrentWave + 1 < Waves.Num())
    {
        // 고정된 휴식 시간
        FTimerHandle NextWaveTimer;
        GetWorldTimerManager().SetTimer(
            NextWaveTimer,
            this,
            &AWaveManager::StartNextWave,
            TimeBetweenWaves,
            false
        );

        UE_LOG(LogTemp, Log, TEXT("Next wave starting in %.1f seconds"), TimeBetweenWaves);
    }
    else
    {
        // 승리!
        UE_LOG(LogTemp, Warning, TEXT("All waves completed! Victory!"));
        OnAllWavesComplete();
    }
}

void AWaveManager::StartNextWave()
{
    CurrentWave++;
    UE_LOG(LogTemp, Warning, TEXT("=== Wave %d Starting! ==="), CurrentWave + 1);
    StartWave();
}

AZombieSpawnVolume* AWaveManager::GetRandomSpawnVolume() const
{
    if (SpawnVolumes.Num() == 0) return nullptr;

    int32 RandomIndex = FMath::RandRange(0, SpawnVolumes.Num() - 1);
    return SpawnVolumes[RandomIndex];
}

void AWaveManager::OnAllWavesComplete_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("=== VICTORY! All waves defeated! ==="));

    // Blueprint에서 추가 처리 가능
    // 예: 승리 UI 표시, 레벨 전환 등
}


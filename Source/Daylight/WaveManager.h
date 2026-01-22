// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

USTRUCT(BlueprintType)
struct FWaveData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ZombieCount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnDelay = 1.0f;  // 좀비 간 스폰 간격

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PrepareTime = 5.0f;  // 웨이브 시작 전 준비 시간
};

UCLASS()
class DAYLIGHT_API AWaveManager : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	AWaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<FWaveData> Waves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<class AZombieSpawnVolume*> SpawnVolumes;

    // 현재 상태
    UPROPERTY(BlueprintReadOnly, Category = "Wave")
    int32 CurrentWave = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Wave")
    int32 RemainingZombies = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Wave")
    bool bWaveInProgress = false;

    // 함수
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartWave();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartNextWave();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void OnZombieKilled();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float TimeBetweenWaves = 10.0f;  // 웨이브 사이 휴식 시간

    UFUNCTION(BlueprintNativeEvent, Category = "Wave")
    void OnAllWavesComplete();
    virtual void OnAllWavesComplete_Implementation();

private:
    FTimerHandle SpawnTimerHandle;
    int32 SpawnedThisWave = 0;

    void SpawnNextZombie();
    void OnWaveComplete();
    class AZombieSpawnVolume* GetRandomSpawnVolume() const;
};

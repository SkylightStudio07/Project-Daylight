// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"  // <- 인터페이스 헤더 추가
#include "DoorHackCrescendo.generated.h"

UCLASS()
class DAYLIGHT_API ADoorHackCrescendo : public AActor, public IInteractable  // <- 인터페이스 상속 추가
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorHackCrescendo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TerminalMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* InteractionBox;

    // 문
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    AActor* DoorActor;

    // 해킹 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hack")
    float HackDuration = 45.f;

    // 웨이브 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<class AZombieSpawnVolume*> SpawnVolumes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 ZombieCount = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float SpawnInterval = 1.0f;

    // 사운드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    class USoundBase* AlarmSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    class USoundBase* HackCompleteSound;

    // 상태
    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsHacking = false;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsCompleted = false;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    float HackProgress = 0.f;

    // 함수
    UFUNCTION(BlueprintCallable, Category = "Hack")
    void StartHack(AActor* HackInstigator);

    UFUNCTION(BlueprintCallable, Category = "Hack")
    void CompleteHack();

    UFUNCTION(BlueprintCallable, Category = "Hack")
    void OpenDoor();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void OnZombieKilled();

    // 이벤트
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnHackStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnHackProgress(float Progress);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnHackCompleted();

    // IInteractable 인터페이스 구현
    virtual bool CanInteract_Implementation(AActor* InteractInstigator) const override;
    virtual void Interact_Implementation(AActor* InteractInstigator) override;
    virtual FString GetInteractionPrompt_Implementation() const override;

protected:
    // 스폰 로직
    void StartSpawning();
    void SpawnNextZombie();
    void UpdateProgress();

    FTimerHandle HackTimerHandle;
    FTimerHandle SpawnTimerHandle;
    FTimerHandle ProgressTimerHandle;

    int32 SpawnedCount = 0;
    int32 KilledCount = 0;
};  
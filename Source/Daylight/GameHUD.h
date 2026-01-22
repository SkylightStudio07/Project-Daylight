// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class DAYLIGHT_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    // 텍스트 블록 (Blueprint에서 바인딩)
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* HealthText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* AmmoText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* WaveText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* ZombiesRemainingText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* Crosshair;

    // 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(float Current, float Max);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateAmmo(int32 Current, int32 Max);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateWave(int32 WaveNumber, int32 ZombiesRemaining);

private:
    class ADaylightCharacter* OwningPlayer;
    class AWaveManager* WaveManager;

    void RefreshPlayerData();
    void RefreshWaveData();
};

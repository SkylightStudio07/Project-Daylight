#include "GameHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "DaylightCharacter.h"
#include "WaveManager.h"
#include "Kismet/GameplayStatics.h"

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 플레이어 찾기
    OwningPlayer = Cast<ADaylightCharacter>(GetOwningPlayerPawn());

    // Wave Manager 찾기
    TArray<AActor*> FoundManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), FoundManagers);
    if (FoundManagers.Num() > 0)
    {
        WaveManager = Cast<AWaveManager>(FoundManagers[0]);
    }
}

void UGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 매 프레임 업데이트
    RefreshPlayerData();
    RefreshWaveData();
}

void UGameHUD::RefreshPlayerData()
{
    if (!OwningPlayer) return;

    // 체력
    UpdateHealth(OwningPlayer->CurrentHealth, OwningPlayer->MaxHealth);

    // 탄약 (나중에 무기 시스템에서 가져오기)
    // UpdateAmmo(CurrentAmmo, MaxAmmo);
}

void UGameHUD::RefreshWaveData()
{
    if (!WaveManager) return;

    UpdateWave(WaveManager->CurrentWave + 1, WaveManager->RemainingZombies);
}

void UGameHUD::UpdateHealth(float Current, float Max)
{
    if (HealthText)
    {
        FString HealthString = FString::Printf(TEXT("%.0f / %.0f"), Current, Max);
        HealthText->SetText(FText::FromString(HealthString));
    }

    if (HealthBar)
    {
        float Percent = (Max > 0) ? (Current / Max) : 0.f;
        HealthBar->SetPercent(Percent);

        // 체력에 따라 색상 변경
        FLinearColor BarColor = FLinearColor::Green;
        if (Percent < 0.3f)
            BarColor = FLinearColor::Red;
        else if (Percent < 0.6f)
            BarColor = FLinearColor::Yellow;

        HealthBar->SetFillColorAndOpacity(BarColor);
    }
}

void UGameHUD::UpdateAmmo(int32 Current, int32 Max)
{
    if (AmmoText)
    {
        FString AmmoString = FString::Printf(TEXT("%d / %d"), Current, Max);
        AmmoText->SetText(FText::FromString(AmmoString));
    }
}

void UGameHUD::UpdateWave(int32 WaveNumber, int32 ZombiesRemaining)
{
    if (WaveText)
    {
        FString WaveString = FString::Printf(TEXT("Wave %d"), WaveNumber);
        WaveText->SetText(FText::FromString(WaveString));
    }

    if (ZombiesRemainingText)
    {
        FString ZombiesString = FString::Printf(TEXT("Zombies: %d"), ZombiesRemaining);
        ZombiesRemainingText->SetText(FText::FromString(ZombiesString));
    }
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "DaylightCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameHUD.h"
#include "Engine/DamageEvents.h"

// Sets default values
ADaylightCharacter::ADaylightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADaylightCharacter::BeginPlay()
{
	Super::BeginPlay();

    CurrentHealth = MaxHealth;

    CurrentHealth = MaxHealth;

    // HUD 생성 (로컬 플레이어만)
    if (IsLocallyControlled())
    {
        if (HUDWidgetClass)
        {
            APlayerController* PC = Cast<APlayerController>(GetController());
            if (PC)
            {
                HUDWidget = CreateWidget<UGameHUD>(PC, HUDWidgetClass);
                if (HUDWidget)
                {
                    HUDWidget->AddToViewport();
                }
            }
        }
    }
	
}

// Called every frame
void ADaylightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADaylightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADaylightCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADaylightCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ADaylightCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &ADaylightCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

}

void ADaylightCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ADaylightCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void ADaylightCharacter::LookUpRate(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("LookUp AxisValue: %f"), AxisValue);
	}
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ADaylightCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

float ADaylightCharacter::TakeDamage(float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    // 이미 죽었으면 무시
    if (IsDead()) return 0.f;

    // 부모 클래스 호출
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // 체력 감소
    CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);

    // 로그
    UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage, Health: %.1f/%.1f"),
        *GetName(), ActualDamage, CurrentHealth, MaxHealth);

    // 죽음 체크
    if (IsDead())
    {
        OnDeath();
    }

    return ActualDamage;
}

FHitResult ADaylightCharacter::PerformWeaponTrace(float Range)
{
    FHitResult OutHit;

    // 카메라 위치
    FVector CameraLoc;
    FRotator CameraRot;
    GetActorEyesViewPoint(CameraLoc, CameraRot);

    FVector Start = CameraLoc;
    FVector End = Start + (CameraRot.Vector() * Range);

    // 충돌 설정
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.bTraceComplex = true;

    // Trace 실행
    GetWorld()->LineTraceSingleByChannel(
        OutHit,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    // 디버그 (개발 중)
#if WITH_EDITOR
    FColor DebugColor = OutHit.bBlockingHit ? FColor::Red : FColor::Green;
    FVector DebugEnd = OutHit.bBlockingHit ? OutHit.Location : End;
    DrawDebugLine(GetWorld(), Start, DebugEnd, DebugColor, false, 2.f, 0, 2.f);

    if (OutHit.bBlockingHit)
    {
        DrawDebugPoint(GetWorld(), OutHit.Location, 10.f, FColor::Red, false, 2.f);
    }
#endif

    return OutHit;
}

void ADaylightCharacter::ProcessWeaponHit(const FHitResult& HitResult)
{
    if (!HitResult.bBlockingHit) return;

    AActor* HitActor = HitResult.GetActor();
    if (!HitActor) return;

    // 데미지 적용
    UGameplayStatics::ApplyPointDamage(
        HitActor,
        BaseDamage,
        HitResult.TraceStart,
        HitResult,
        GetController(),
        this,
        UDamageType::StaticClass()
    );

    UE_LOG(LogTemp, Warning, TEXT("Hit: %s at %s"),
        *HitActor->GetName(),
        *HitResult.ImpactPoint.ToString());
}

void ADaylightCharacter::ApplyDamage(float Damage)
{
    // 간단한 래퍼 함수
    TakeDamage(Damage, FDamageEvent(), nullptr, nullptr);
}

void ADaylightCharacter::OnDeath_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("%s died!"), *GetName());

    // 죽음 처리 (BP에서 오버라이드 가능)
    // 예: Ragdoll, 애니메이션, 디스폰 등

    // 임시: 캐릭터 비활성화
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetSimulatePhysics(true);
}
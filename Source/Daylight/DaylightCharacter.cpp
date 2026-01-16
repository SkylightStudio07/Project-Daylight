// Fill out your copyright notice in the Description page of Project Settings.


#include "DaylightCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

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

bool ADaylightCharacter::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

bool ADaylightCharacter::PerformHitscan(float Range, FHitResult& OutHit)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	// Get the player's viewpoint
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Calculate the end location of the trace
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * Range);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	// Perform the line trace (raycast)
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	#if WITH_EDITOR
    if (bHit)
    {
        DrawDebugLine(GetWorld(), TraceStart, OutHit.Location, FColor::Red, false, 2.f, 0, 2.f);
        DrawDebugPoint(GetWorld(), OutHit.Location, 10.f, FColor::Red, false, 2.f);
    }
    else
    {
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.f, 0, 2.f);
    }
    #endif
    
    return bHit;
}

void ADaylightCharacter::ApplyWeaponDamage(AActor* HitActor, const FHitResult& HitResult)
{
	if (!HitActor) return;

	// 데미지 적용
	UGameplayStatics::ApplyPointDamage(
		HitActor,
		WeaponDamage,
		HitResult.TraceStart,
		HitResult,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	UE_LOG(LogTemp, Warning, TEXT("Hit: %s for %.1f damage"), *HitActor->GetName(), WeaponDamage);
}
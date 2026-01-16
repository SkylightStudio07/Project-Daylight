// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DaylightCharacter.generated.h"

UCLASS()
class DAYLIGHT_API ADaylightCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADaylightCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool PerformHitscan(float Range, FHitResult& OutHit);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyWeaponDamage(AActor* HitActor, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float WeaponDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float WeaponRange = 10000.f;

private:
	
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	UPROPERTY(EditAnywhere)
	float RotationRate = 20.0f;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;



};

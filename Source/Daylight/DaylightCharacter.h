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
	
	virtual float TakeDamage(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// BP에서 호출 가능한 함수들
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return CurrentHealth <= 0.f; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }


	UFUNCTION(BlueprintCallable, Category = "Combat")
	FHitResult PerformWeaponTrace(float Range = 10000.f);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ProcessWeaponHit(const FHitResult& HitResult);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseDamage = 20.f;

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
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();
	virtual void OnDeath_Implementation();


};

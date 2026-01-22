// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieBase.h"
#include "ZombieSpawnVolume.generated.h"

UCLASS()
class DAYLIGHT_API AZombieSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	class UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class AZombieBase> ZombieClass;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AZombieBase* SpawnZombie();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnZombies(int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomPointInVolume() const;

private:
	bool FindValidSpawnLocation(FVector& OutLocation) const;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class DAYLIGHT_API ADoor : public AActor
{
    GENERATED_BODY()

public:
    ADoor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* DoorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    float OpenAngle = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    float OpenSpeed = 90.f;  // 초당 각도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    bool bOpenDirection = true;  // true: +Y, false: -Y

    UPROPERTY(BlueprintReadOnly, Category = "Door")
    bool bIsOpen = false;

    UPROPERTY(BlueprintReadOnly, Category = "Door")
    bool bIsOpening = false;

    UFUNCTION(BlueprintCallable, Category = "Door")
    void OpenDoor();

    UFUNCTION(BlueprintImplementableEvent, Category = "Door")
    void OnDoorOpened();

private:
    FRotator InitialRotation;
    FRotator TargetRotation;
    float CurrentOpenAmount = 0.f;

};

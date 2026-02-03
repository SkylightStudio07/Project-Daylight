#include "Door.h"

ADoor::ADoor()
{
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent);
}

void ADoor::BeginPlay()
{
    Super::BeginPlay();

    InitialRotation = DoorMesh->GetRelativeRotation();
    TargetRotation = InitialRotation;
    TargetRotation.Yaw += (bOpenDirection ? OpenAngle : -OpenAngle);
}

void ADoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsOpening)
    {
        CurrentOpenAmount += DeltaTime * OpenSpeed;

        if (CurrentOpenAmount >= OpenAngle)
        {
            CurrentOpenAmount = OpenAngle;
            bIsOpening = false;
            bIsOpen = true;

            DoorMesh->SetRelativeRotation(TargetRotation);

            UE_LOG(LogTemp, Warning, TEXT("Door fully opened"));
            OnDoorOpened();

            // Tick 비활성화 (최적화)
            SetActorTickEnabled(false);
        }
        else
        {
            // 부드러운 보간
            float Alpha = CurrentOpenAmount / OpenAngle;
            FRotator CurrentRot = FMath::Lerp(InitialRotation, TargetRotation, Alpha);
            DoorMesh->SetRelativeRotation(CurrentRot);
        }
    }
}

void ADoor::OpenDoor()
{
    if (bIsOpen || bIsOpening)
    {
        UE_LOG(LogTemp, Warning, TEXT("Door already open or opening"));
        return;
    }

    bIsOpening = true;
    SetActorTickEnabled(true);

    UE_LOG(LogTemp, Warning, TEXT("Door opening..."));
}
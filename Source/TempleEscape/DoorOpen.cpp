// Copyright Cheah Jing Feng 2020


#include "DoorOpen.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UDoorOpen::UDoorOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}

void UDoorOpen::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing the Audio Component!"), *GetOwner()->GetName());
	}
}

void UDoorOpen::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it but no pressure plate set!"), *GetOwner()->GetName());
	}
}

void UDoorOpen::PlayDoorAudio() const
{
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Audio Component."), *GetOwner()->GetName());
		return;
	}

	AudioComponent->Play();
}

// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() >= MassToOpenDoors)
	{
		DoorOpen(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();

		// Play Door Opening Sound
		if (!bDoorOpen)
		{
			PlayDoorAudio();
			bDoorOpen = true;
		}
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);

			// Play Door Opening Sound
			if (bDoorOpen)
			{
				PlayDoorAudio();
				bDoorOpen = false;
			}
		}
	}
}

void UDoorOpen::DoorOpen(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent)
	{
		return;
	}
}

void UDoorOpen::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent)
	{
		return;
	}
}

float UDoorOpen::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find All Overlapping Actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add Up Their Masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
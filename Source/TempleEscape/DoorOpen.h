// Copyright Cheah Jing Feng 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorOpen.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEMPLEESCAPE_API UDoorOpen : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDoorOpen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DoorOpen(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();
	void PlayDoorAudio() const;

private:
	float InitialYaw;
	float CurrentYaw;

	bool bDoorOpen = false;

	UPROPERTY(EditAnywhere)
		float MassToOpenDoors = 50.f;

	UPROPERTY(EditAnywhere)
		float OpenAngle = 90.f;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = .8f;

	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
		UAudioComponent* AudioComponent = nullptr;
};

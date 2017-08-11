// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
	//Find the owning Actor
	Owner = GetOwner();	

	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is not found"));
	}

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate Component is not found"));
	}
}

void UOpenDoor::OpenDoor()
{		
	//Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{	
	//Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the trigger Volume	
	if (GetTotalMassOfActorsOnPlate() >= 50.0f) //TODO change the magic number
	{		
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}	

	//Check if its time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >  DoorCloseDelay)
	{
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* Actor : OverlappingActors)
	{		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s in on the pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}



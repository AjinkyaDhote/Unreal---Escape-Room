// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber is here"));

	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	///Log
	//UE_LOG(LogTemp, Warning, TEXT("Position is %s"), *PlayerViewLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Rotation is %s"), *PlayerViewRotation.ToString());

	///How long should be the ray drawn
	FVector LineReachEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;

	///Draw Debug Ray
	DrawDebugLine(GetWorld(), PlayerViewLocation, LineReachEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);


	///Setup Query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Ray Casting
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewLocation,
		LineReachEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);

	
	AActor* ObjectHit = Hit.GetActor();

	if (ObjectHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Hit is %s"), *(ObjectHit->GetName()));
	}	 
}


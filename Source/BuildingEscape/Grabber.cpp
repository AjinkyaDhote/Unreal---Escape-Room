//Copyright Ajinkya Dhote

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();		
}

void UGrabber::Grab()
{		
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //gets the mesh component in this case
	auto ActorHit = HitResult.GetActor();

	///If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}	
}

void UGrabber::Release()
{	
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object we are holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
		
}

///Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)	
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle component is missing on %s"), *GetOwner()->GetName());
	}
}

///Look for attached Input Component
void UGrabber::SetupInputComponent()
{
	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{	
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{		
		UE_LOG(LogTemp, Error, TEXT("Input component is missing on %s"), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{				
	FHitResult Hit;
	///Setup Query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);

	AActor* ObjectHit = Hit.GetActor();

	if (ObjectHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Hit is %s"), *(ObjectHit->GetName()));
	}
	return Hit;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

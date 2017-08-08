//Copyright Ajinkya Dhote

#pragma once
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
private :	
	//PlayerReach
	float Reach = 100.0f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	FVector LineReachEnd;


	//Raycast and grab the object within reach
	void Grab();

	//Release the Grabbed Object
	void Release();

	//Find attached Physics Handle Component
	void FindPhysicsHandleComponent();

	//Setup attached input Component
	void SetupInputComponent();

	//Return Hit for first Physics Body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Returns start of Reach Line
	FVector GetReachLineStart();

	//Returns Current end of Reach Line
	FVector GetReachLineEnd();
};

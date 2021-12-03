// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

// Sets default values
AVRPawn::AVRPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	LineTraceCursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LineTraceCursor"));

	RootComponent = _root;
	_camera->SetupAttachment(_root);
	LeftController->SetupAttachment(_root);
	RightController->SetupAttachment(_root);
	LineTraceCursor->SetupAttachment(_root);

	LeftController->MotionSource = FName("Left");
	RightController->MotionSource = FName("Right");
	LeftController->bDisplayDeviceModel = true;
	RightController->bDisplayDeviceModel = true;

	LineTraceCursor->SetVisibility(false);

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the user is pressing the line trace buton, do the line trace.
	if (_isLineTracing) {
		// Use the right controller to aim the line trace.
		FVector start = RightController->GetComponentLocation();
		FVector direction = RightController->GetForwardVector();
		FVector end = start + (direction * LineTraceLength);

		ECollisionChannel channel = ECollisionChannel::ECC_WorldStatic;
		FCollisionQueryParams params(FName(TEXT("")), true, this);
		FHitResult hit;

		if (GetWorld()->LineTraceSingleByChannel(OUT hit, start, end, channel, params)) {
			// If the line trace hit something, move the cursor to that spot.
			_isLineTraceHitting = true;
			_lineTraceHitLocation = hit.Location;
			LineTraceCursor->SetVisibility(true);
			LineTraceCursor->SetWorldLocation(_lineTraceHitLocation);
		}
		else {
			// If the line trace didn't hit anything, hide the cursor.
			_isLineTraceHitting = false;
			LineTraceCursor->SetVisibility(false);
		}
	}
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("LineTrace", IE_Pressed, this, &AVRPawn::_startLineTracing);
	InputComponent->BindAction("LineTrace", IE_Released, this, &AVRPawn::_stopLineTracing);
	InputComponent->BindAction("Teleport", IE_Pressed, this, &AVRPawn::_teleport);

}

void AVRPawn::_startLineTracing() { _isLineTracing = true; }

void AVRPawn::_stopLineTracing() {
	_isLineTracing = false;
	_isLineTraceHitting = false;
	LineTraceCursor->SetVisibility(false);

}

void AVRPawn::_teleport() {
	if (!_isLineTracing || !_isLineTraceHitting) {
		return;
	}

	FVector newLocation = _lineTraceHitLocation;
	newLocation.Z += OffsetFromGround;
	SetActorLocation(newLocation);

}
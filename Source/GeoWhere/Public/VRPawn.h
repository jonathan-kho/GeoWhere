// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Camera/CameraComponent.h>
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>
#include <MotionControllerComponent.h>

#include "VRPawn.generated.h"

UCLASS()
class GEOWHERE_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMotionControllerComponent* LeftController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UMotionControllerComponent* RightController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* LineTraceCursor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LineTraceLength = 100000000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float OffsetFromGround = 4000;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void _startLineTracing();
	void _stopLineTracing();
	void _teleport();

	UPROPERTY()
		USceneComponent* _root;

	UPROPERTY()
		UCameraComponent* _camera;

	bool _isLineTracing;
	bool _isLineTraceHitting;
	FVector _lineTraceHitLocation;

};

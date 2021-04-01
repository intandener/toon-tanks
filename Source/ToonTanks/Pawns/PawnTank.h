// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 100.f;

	APlayerController* PlayerController;

	FRotator Rotation;
	FVector Movement;

	bool bIsAlive;

	void Rotate();
	void Move();

	void CalculateRotation(float Value);
	void CalculateMovement(float Value);

public:
	APawnTank();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction() override;

	bool IsAlive() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

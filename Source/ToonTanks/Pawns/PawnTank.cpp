// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ToonTanks/Vehicle.h"
#include "ToonTanks/Truck.h"
#include "ToonTanks/Car.h"

APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetController<APlayerController>();
	bIsAlive = true;

	// UClass Tests
	UCar* Car = NewObject<UCar>();
	UTruck* Truck = NewObject<UTruck>();
	UVehicle* VehicleCar = Car;
	UVehicle* Vehicle = NewObject<UVehicle>();

	if (TSubclassOf<UVehicle>(Car->GetClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UVehicle>(Car->GetClass()): true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UVehicle>(Car->GetClass()): false"));
	}
	if (TSubclassOf<UVehicle>(VehicleCar->GetClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UVehicle>(VehicleCar->GetClass()): true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UVehicle>(VehicleCar->GetClass()): false"));
	}

	if (TSubclassOf<UCar>(VehicleCar->GetClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UCar>(VehicleCar->GetClass()): true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UCar>(VehicleCar->GetClass()): false"));
	}
	if (TSubclassOf<UCar>(Vehicle->GetClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UCar>(Vehicle->GetClass()): true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TSubclassOf<UCar>(Vehicle->GetClass()): false"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Car->GetClass(): %s"), *Car->GetClass()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("VehicleCar->GetClass(): %s"), *VehicleCar->GetClass()->GetName());
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate();
	Move();

	// rotate turret
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawnTank::CalculateRotation);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnTank::CalculateMovement);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
	bIsAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(Rotation, true);
}

void APawnTank::Move()
{
	AddActorLocalOffset(Movement, true);
}

void APawnTank::CalculateRotation(float Value)
{
	Rotation = FRotator(0.f, Value * RotationSpeed * GetWorld()->DeltaTimeSeconds, 0.f);
}

void APawnTank::CalculateMovement(float Value)
{
	Movement = FVector(Value * MovementSpeed * GetWorld()->DeltaTimeSeconds, 0.f, 0.f);
}

bool APawnTank::IsAlive() const
{
	return bIsAlive;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "ToonTanks/Components/HealthComponent.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

/**
 * Checks if all Health components of this actor have reached zero health
 */
bool APawnBase::IsDead()
{
	UE_LOG(LogTemp, Display, TEXT("Checking IsDead; HealthComponents health is %f"), HealthComponent->GetHealth());
	return HealthComponent->GetHealth() <= 0.f;
}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	FVector TurretLocation = TurretMesh->GetComponentLocation();
	FVector LookAtTargetCleaned(LookAtTarget.X, LookAtTarget.Y, TurretLocation.Z);
	FRotator TargetRotation = (LookAtTargetCleaned - TurretLocation).Rotation();
	TurretMesh->SetWorldRotation(TargetRotation);
}
void APawnBase::Fire()
{
	if (ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		Projectile->SetOwner(this);
	}
}
void APawnBase::HandleDestruction()
{
	// Spawn death particle effect
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathShake);
}


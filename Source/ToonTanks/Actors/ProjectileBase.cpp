// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;

	ProjectileTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Trail"));
	ProjectileTrail->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(DespawnAfter);
	// ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnBeginOverlap);
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Bullet is beginning to overlap %s with othercomp %s of actor %s  bodyIndex: %i, fromsweep: %i"), *OverlappedComponent->GetName(), *OtherComp->GetName(), *OtherActor->GetName(), OtherBodyIndex, bFromSweep);
	AActor* ProjectileOwner = GetOwner();
	if (ProjectileOwner && OtherActor && OtherActor != ProjectileOwner)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, SweepResult.ImpactPoint);
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, Damage, ProjectileOwner->GetInstigatorController(), this, DamageType);

		// Camera Shake only if it hits the player
		if(GetWorld()->GetFirstPlayerController()->GetPawn() == OtherActor)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitShake);
		}
		
		Destroy();
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Bullet is colliding with comp %s of actor %s"), *OtherActor->GetName(), *OtherComp->GetName());
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	if (OtherActor && OtherActor != MyOwner)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, Hit.ImpactPoint);
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		Destroy();
	}	
}

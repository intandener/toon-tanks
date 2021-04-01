// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"


void APawnTurret::Tick(float DeltaTime)
{
	if (!PlayerActor || GetDistanceTo(PlayerActor) > FireRange) return;

	RotateTurret(PlayerActor->GetActorLocation());
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerActor = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, /*Loop*/ true);
}

void APawnTurret::CheckFireCondition()
{
	if (PlayerActor && PlayerActor->IsAlive() && GetDistanceTo(PlayerActor) <= FireRange)
	{
		Fire();
	}
}

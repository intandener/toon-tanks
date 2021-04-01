// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}
void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	UE_LOG(LogTemp, Error, TEXT("Actor %s died! Class: %s"), *DeadActor->GetName(), *DeadActor->GetClass()->GetDefaultObjectName().ToString());

	
	if (APawnTank* DestroyedTank = Cast<APawnTank>(DeadActor))
	{
		DestroyedTank->HandleDestruction();
		HandleGameOver(false);
	}
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
	{
		DestroyedTurret->HandleDestruction();

		int32 NumAliveTurrets = GetNumAliveTurrets();
		OnAliveTurretsChanged(NumAliveTurrets);
		
		if (NumAliveTurrets == 0)
		{
			HandleGameOver(true);
		}
	}
}
void ATankGameModeBase::HandleGameStart()
{
	GameStart();
	int32 NumAliveTurrets = GetNumAliveTurrets();
	UE_LOG(LogTemp, Display, TEXT("At the start of the game %i turrets are alive."), NumAliveTurrets);
	OnAliveTurretsChanged(NumAliveTurrets);
	
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerController->SetPlayerEnableState(false);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(PlayerController, &APlayerControllerBase::SetPlayerEnableState, true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, StartDelay, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Game Started!"));
}
void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
	GameOver(bPlayerWon);
	if (!bPlayerWon)
	{
		PlayerController->SetPlayerEnableState(false);
	}
	UE_LOG(LogTemp, Warning, TEXT("Game Over! Won: %i"), bPlayerWon);
}

int32 ATankGameModeBase::GetNumAliveTurrets() const
{
	TArray<AActor*> Turrets;

	// StaticClass syntax
	UGameplayStatics::GetAllActorsOfClass(this, APawnTurret::StaticClass(), Turrets);

	// TSubclassOf syntax. This does NOT work!
	// UGameplayStatics::GetAllActorsOfClass(this, TSubclassOf<APawnTurret>(), Turrets);

	int32 AliveTurrets = 0;
	for (AActor* Turret : Turrets)
	{
		if (!Cast<APawnTurret>(Turret)->IsDead())
		{
			++AliveTurrets;
		}
	}
	return AliveTurrets;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"


void APlayerControllerBase::SetPlayerEnableState(bool bSetPlayerEnabled)
{
	UE_LOG(LogTemp, Error, TEXT("Setting Player Controller Input enabled: %i"), bSetPlayerEnabled);
	if (bSetPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}

	bShowMouseCursor = bSetPlayerEnabled;
}

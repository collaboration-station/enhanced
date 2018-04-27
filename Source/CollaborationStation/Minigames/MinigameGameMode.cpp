//  Copyright (c) 2018 Paul Gestwicki
//
//  This file is part of Collaboration Station Enhanced.
//
//  Collaboration Station Enhanced is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Collaboration Station Enhanced is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Collaboration Station Enhanced.  If not, see <http://www.gnu.org/licenses/>.
//
#include "MinigameGameMode.h"
#include "Engine.h"
#include "CollaborationStationGameInstance.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Multiplayer/MinigameNetworkAdapter.h"

AMinigameGameMode::AMinigameGameMode() 
	: MinigameState(EMinigameState::Playing)
	, Score(0)
	, bPauseOnEnd(true)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMinigameGameMode::PostInitProperties()
{
	AGameModeBase::PostInitProperties();
	RoundDuration = FTimespan::FromSeconds(RoundDurationSeconds);
	TimeRemainingInRound = RoundDuration;
}


void AMinigameGameMode::BeginPlay()
{
	// Unfortunately, when I tried using methods to load the network adapter by name, they all
	// failed: they would---very strangely---make the gameDNA plugins fail to load.
	// So, we're left with this: a reminder that you have to specify network adapters.
	check(NetworkAdapter && "MinigameGameModes *must* have a network adapter specified.");

	RoundStartTime = FDateTime::UtcNow();
	UCollaborationStationGameInstance* GameInstance = (UCollaborationStationGameInstance*)GetGameInstance();
	if (GameInstance)
	{
		if (GameInstance->bTraining)
		{
			// Set up the starting time
			RoundStartTime = FDateTime::UtcNow();

			// Set my country
			GameInstance->Country = ECountry::USA;
		}
		else
		{
			AMinigameNetworkAdapter* networkAdapter = GetWorld()->SpawnActor<AMinigameNetworkAdapter>(NetworkAdapter, FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
			OnStateChange.AddDynamic(networkAdapter, &AMinigameNetworkAdapter::OnMinigameStateChange);
		}
	}
	Super::BeginPlay();
}

int32 AMinigameGameMode::GetScore()
{
	return Score;
}

int32 AMinigameGameMode::AddPoints(int32 Points)
{
	int32 OldScore = Score;
	Score += Points;
	OnScoreChange.Broadcast(Score, OldScore);
	return Score;
}

void AMinigameGameMode::Tick(float DeltaTime)
{
	FTimespan Elapsed = FDateTime::UtcNow() - RoundStartTime;

	switch (MinigameState)
	{
	case EMinigameState::Playing:
		TimeRemainingInRound = RoundDuration - Elapsed;
		if (TimeRemainingInRound <= FTimespan::Zero())
		{
			if (bPauseOnEnd)
			{
				APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
				if (PlayerController)
				{
					PlayerController->Pause();
				}
			}
			ChangeState(EMinigameState::Done);
			UE_LOG(LogTemp, Warning, TEXT("Done with minigame"));
		}
		break;
	}
}

void AMinigameGameMode::ChangeState(EMinigameState NewState)
{
	check(NewState != MinigameState);
	EMinigameState OldState = MinigameState;
	MinigameState = NewState;
	OnStateChange.Broadcast(MinigameState, OldState);
}

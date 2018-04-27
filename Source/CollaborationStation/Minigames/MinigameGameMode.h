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
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MinigameGameMode.generated.h"

UENUM(BlueprintType)
enum class EMinigameState : uint8
{
	Playing,
	Done
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChange, EMinigameState, NewState, EMinigameState, OldState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChange, int32, NewScore, int32, OldScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDebriefAvailable);

/**
 * Base GameMode for all Collaboration Station minigames.
 */
UCLASS(config=Game)
class COLLABORATIONSTATION_API AMinigameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AMinigameGameMode();

	// Configure this based on settings
	//
	// This is the first point where we can read settings from the configuration files,
	// not in the constructor.
	virtual void PostInitProperties() override;

public:
	// Duration of a round of the game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Configuration")
	FTimespan RoundDuration;

	// Time remaining in this round
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimespan TimeRemainingInRound;

	// The time that the round will start (UTC)
	//
	// This is needed because a mobile device may background the game but then come back,
	// and we may not get tick messages while the game is gone. Hence, we cannot simply
	// rely on Tick's counting down of seconds.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDateTime RoundStartTime;

	// Notifies delegates when the state changes
	UPROPERTY(BlueprintAssignable)
	FOnStateChange OnStateChange;

	// Notifies delegates when the score changes
	UPROPERTY(BlueprintAssignable)
	FOnScoreChange OnScoreChange;

	// Points earned in this minigame
	UFUNCTION(BlueprintCallable,BlueprintPure)
	int32 GetScore();

	// Add points to the current point total. Returns the new score.
	UFUNCTION(BlueprintCallable)
	int32 AddPoints(int32 Points);

	// Called by the system to indicate that the debrief is ready (i.e. all players
	// have finished the round) and so the player should be allowed to go
	// to the debriefing.
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDebriefAvailable OnDebriefAvailable;

	// The on-screen name of this minigame.
	// This must be set by blueprint subclasses of this class.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText Name;


protected:
	void Tick(float DeltaTime);

	virtual void BeginPlay() override;

	// An adapter class that is spawned when the minigame starts.
	// It will be connected to the state change notification within the
	// minigame so that it can appropriately watch Firebase and update its
	// state there.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class AMinigameNetworkAdapter> NetworkAdapter;

	int32 Score;

	// Indicates if the minigame should automatically pause the game when
	// the timer ends. Default is true.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bPauseOnEnd;

private:
	// The current state of the minigame
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess="true"))
	EMinigameState MinigameState;

	// Change state and notify listeners
	void ChangeState(EMinigameState NewState);

	UPROPERTY(config)
	float RoundDurationSeconds;
};

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
#include "Engine/DataAsset.h"
#include "MultiplayerGameGoals.generated.h"

/**
 * Goals for a multiplayer game.
 * The length of the array specifies the number of rounds.
 */
UCLASS(BlueprintType)
class COLLABORATIONSTATION_API UMultiplayerGameGoals : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<int32> Points;
	
	// Compute the score goal.
	// Round is 1-based (round 1, round 2) to match multiplayer network protocol.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 ComputeGoal(int32 Round, int32 Players);
};

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
#include "Minigames/MinigameGameMode.h"
#include "MemoryGameMode.generated.h"

/**
 * Memory minigame game mode
 */
UCLASS(config=Game)
class COLLABORATIONSTATION_API AMemoryGameMode : public AMinigameGameMode
{
	GENERATED_BODY()
	
protected:

	AMemoryGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	TSubclassOf<class AGrid> GridClass;

	UPROPERTY(EditDefaultsOnly, Category="Configuration")
	FVector GridSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	FVector2D GridSize;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	uint32 bShuffle : 1;

public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(config)
	int32 PointsPerMatch;
};

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
#include "GameFramework/Actor.h"
#include "SlidingTile.generated.h"

UCLASS()
class COLLABORATIONSTATION_API ASlidingTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingTile();

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	class UMaterialInstanceDynamic* Material;

	// The starting position of this tile within the puzzle
	FIntPoint StartPosition;

	// The logical position of this tile within the puzzle.
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	FIntPoint Position;

	// The puzzle of which this tile is a part
	UPROPERTY(BlueprintReadOnly)
	class ASlidingTilePuzzle* Puzzle;
	
};

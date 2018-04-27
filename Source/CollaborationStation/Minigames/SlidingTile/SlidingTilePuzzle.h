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
#include "SlidingTilePuzzle.generated.h"

class ASlidingTile;

UCLASS()
class COLLABORATIONSTATION_API ASlidingTilePuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingTilePuzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	class UMaterialInterface* Material;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<ASlidingTile> TileClass;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	TArray<ASlidingTile*> Tiles;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	FIntPoint Gap;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	int32 bHasGap;

	// Create the gap that makes the puzzle playable.
	// Returns true if everything went correctly.
	UFUNCTION(BlueprintCallable)
	bool RemoveTileToCreateGap(ASlidingTile* Tile);

	// Put the tile back into the array for the grand conclusion
	UFUNCTION(BlueprintCallable)
	bool ReinsertGapTile(ASlidingTile* Tile);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanMove(ASlidingTile* Tile);

	// Move a tile.
	// Return true if it moved, false if it's not legal to move it.
	UFUNCTION(BlueprintCallable)
	bool Move(ASlidingTile* Tile);

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SlidingSound;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTileMoved(ASlidingTile* Tile, FIntPoint From, FIntPoint To);

	// Check if the puzzle is complete.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSolved();

	// Choose a random movable tile (a tile next to a gap).
	// This will not return the same tile twice in a row.
	UFUNCTION(BlueprintCallable)
	ASlidingTile* GetRandomMovableTile();

private:
	bool IsOnMap(FIntPoint Point);

	ASlidingTile* GetTileAt(FIntPoint Point);

	TArray<FIntPoint> NeighborOffsets;

	// The tile previously chosen randomly. 
	// This reference is held so that we can ensure we don't return the same tile twice in a row during shuffling.
	ASlidingTile* PreviousRandomTile;
};

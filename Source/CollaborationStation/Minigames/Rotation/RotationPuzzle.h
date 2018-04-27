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
#include "RotationPuzzle.generated.h"

struct FRotatableTileArray
{
	TArray<ARotatableTile*> Children;
};

UCLASS()
class COLLABORATIONSTATION_API ARotationPuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotationPuzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Puzzle Configuration")
	int32 MaxRows;
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle Configuration")
	int32 MaxColumns;

	// The tiles that comprise the puzzle
	TArray<FRotatableTileArray> Columns;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<ARotatableTile> TileClass;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARotatableTile* GetTile(int32 Column, int32 Row);

	// Called when the puzzle is complete.
	UFUNCTION(BlueprintImplementableEvent)
	void OnPuzzleComplete();

public:	
	// Determine if a given tile in this puzzle has a path to the source.
	bool HasPathToSource(ARotatableTile* Tile);

	// Update all the tiles' glow status.
	void UpdateTiles();

private:
	// Generate the map, setting the paths for each tile before they finish spawning
	void GeneratePuzzle();

	// Helper method for recursing through the path to the source
	bool HasPathToSource(ARotatableTile* Tile, UPARAM(ref) TArray<ARotatableTile*>& VisitedTiles);
	
	// Check if the puzzle is complete
	bool IsComplete();
};

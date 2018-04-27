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
#include "RotationPuzzle.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "RotatableTile.h"

// Sets default values
ARotationPuzzle::ARotationPuzzle() :
	MaxRows(4),
	MaxColumns(4)
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARotationPuzzle::BeginPlay()
{
	if (!TileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("You did not initialize TileClass"));
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		// Start deferred spawning of all tiles.
		FTransform Transform;
		for (int i = 0; i < MaxColumns; i++)
		{
			FRotatableTileArray Column;
			for (int j = 0; j < MaxRows; j++)
			{
				ARotatableTile* Tile = World->SpawnActorDeferred<ARotatableTile>(TileClass, Transform, this);
				Tile->PuzzlePosition = FIntPoint(i, j);
				Column.Children.Add(Tile);
			}
			Columns.Add(Column);
		}

		// Generate the map
		GeneratePuzzle();
		
		// Finish deferred spawning.
		for (int i = 0; i < MaxColumns; i++)
		{
			for (int j = 0; j < MaxRows; j++)
			{
				ARotatableTile* Tile = GetTile(i, j);
				// Set the puzzle just before finishing deferred spawning so that
				// turning the tiles during shuffling does not trigger updating status.
				Tile->Puzzle = this;
				UGameplayStatics::FinishSpawningActor(Tile, Transform);
			}
		}
		UpdateTiles();
	}
	Super::BeginPlay();
}

ARotatableTile * ARotationPuzzle::GetTile(int32 Column, int32 Row)
{
	return Columns[Column].Children[Row];
}

bool ARotationPuzzle::HasPathToSource(ARotatableTile * Tile)
{
	TArray<ARotatableTile*> VisitedTiles;
	return HasPathToSource(Tile, VisitedTiles);
}

void ARotationPuzzle::UpdateTiles()
{
	for (int i = 0; i < MaxColumns; i++)
	{
		for (int j = 0; j < MaxRows; j++)
		{
			bool bHasPath = GetTile(i, j)->UpdateGlowStatus();
			if ((i == MaxColumns - 1) && (j == MaxRows - 1) && IsComplete())
			{
				OnPuzzleComplete();
			}
		}
	}
}

void ARotationPuzzle::GeneratePuzzle()
{
	TArray<FIntPoint> Path;
	FIntPoint Current(0, 0);
	Path.Add(Current);
	do {
		EPathDirection NextStepDirection;
		do {
			NextStepDirection = static_cast<EPathDirection>(FMath::RandRange(0, 3));
		} while ((NextStepDirection == EPathDirection::Left && Current.X == 0)
			|| (NextStepDirection == EPathDirection::Up && Current.Y == 0)
			|| (NextStepDirection == EPathDirection::Down && Current.Y == MaxRows - 1)
			|| (NextStepDirection == EPathDirection::Right && Current.X == MaxColumns - 1));

		FIntPoint Next;
		switch (NextStepDirection)
		{
		case EPathDirection::Down:
			Next = FIntPoint(Current.X, Current.Y + 1); break;
		case EPathDirection::Left:
			Next = FIntPoint(Current.X - 1, Current.Y); break;
		case EPathDirection::Up:
			Next = FIntPoint(Current.X, Current.Y - 1); break;
		case EPathDirection::Right:
			Next = FIntPoint(Current.X + 1, Current.Y); break;
		}
		Path.Add(Next);
		Current = Next;
	} while (Current != FIntPoint(MaxColumns - 1, MaxRows - 1));

	// Copy the generated directions into the tile paths.
	for (int i=0; i<Path.Num()-1; i++)
	{
		FIntPoint Point = Path[i];
		ARotatableTile* Tile = GetTile(Point.X, Point.Y);
		FIntPoint NextPoint = Path[i + 1];
		ARotatableTile* NextTile = GetTile(NextPoint.X, NextPoint.Y);
		if (NextPoint.X == Point.X - 1)
		{
			Tile->Paths.Add(EPathDirection::Left);
			NextTile->Paths.Add(EPathDirection::Right);
		}
		else if (NextPoint.X == Point.X + 1)
		{
			Tile->Paths.Add(EPathDirection::Right);
			NextTile->Paths.Add(EPathDirection::Left);
		}
		else if (NextPoint.Y == Point.Y - 1)
		{
			Tile->Paths.Add(EPathDirection::Up);
			NextTile->Paths.Add(EPathDirection::Down);
		}
		else 
		{
			Tile->Paths.Add(EPathDirection::Down);
			NextTile->Paths.Add(EPathDirection::Up);
		}
	}
	// Connect the first and last tiles to the source and sink
	GetTile(0, 0)->Paths.Add(EPathDirection::Left);
	GetTile(MaxColumns - 1, MaxRows - 1)->Paths.Add(EPathDirection::Right);

	// Dump all the path coordinates into a map for faster indexing
	TSet<FIntPoint> PointSet;
	for (auto &Point : Path)
	{
		PointSet.Add(Point);
	}

	// Go through all possible points. Fill in each tile with 2 or 3 paths if it doesn't have them already.
	for (int i = 0; i < MaxColumns; i++)
	{
		for (int j = 0; j < MaxRows; j++)
		{
			ARotatableTile* Tile = GetTile(i, j);
			int32 TargetPaths = FMath::RandRange(2, 3);
			while (Tile->Paths.Num() < TargetPaths)
			{
				EPathDirection Direction = static_cast<EPathDirection>(FMath::RandRange(0, 3));
				Tile->Paths.Add(Direction);
			}

			// I think that if we have four paths, we can always remove one and make a solvable puzzle,
			// since it means we crossed paths, and so there is another way.
			if (Tile->Paths.Num() == 4)
			{
				EPathDirection Direction = static_cast<EPathDirection>(FMath::RandRange(0, 3));
				Tile->Paths.Remove(Direction);
			}

			UE_LOG(LogTemp, Warning, TEXT("Tile %d %d now has %d paths out"), i, j, Tile->Paths.Num());
		}
	}

	// Shuffle it up
	do 	{
		for (int i = 0; i < MaxColumns; i++)
		{
			for (int j = 0; j < MaxRows; j++)
			{
				ARotatableTile* Tile = GetTile(i, j);
				int32 Turns = FMath::RandRange(0, 3);
				while (Turns > 0)
				{
					Tile->Turn();
					Turns--;
				}
			}
		}
	} while (IsComplete());
}

bool ARotationPuzzle::HasPathToSource(ARotatableTile * Tile, UPARAM(ref) TArray<ARotatableTile*>& VisitedTiles) 
{
	// Upper-left tile has a path if it can go left.
	if (Tile->PuzzlePosition == FIntPoint(0, 0))
	{
		UE_LOG(LogTemp, Display, TEXT("Found origin case"));
		return Tile->Paths.Contains(EPathDirection::Left);
	}

	// If we have already visited this tile, then this cannot be represent a valid path, 
	// though there may be another one.
	if (VisitedTiles.Contains(Tile))
	{
		return false;
	}

	// Otherwise, track this tile as visited and recursively search.
	VisitedTiles.Add(Tile);
	FIntPoint Position = Tile->PuzzlePosition;
	if (Tile->Paths.Contains(EPathDirection::Left)
		&& Tile->PuzzlePosition.X > 0)
	{
		ARotatableTile* Neighbor = GetTile(Position.X-1, Position.Y);
		if (Neighbor->Paths.Contains(EPathDirection::Right) && !VisitedTiles.Contains(Neighbor))
		{
			if (HasPathToSource(Neighbor, VisitedTiles))
			{
				return true;
			}
		}
	}
	if (Tile->Paths.Contains(EPathDirection::Up)
		&& Tile->PuzzlePosition.Y > 0)
	{
		ARotatableTile* Neighbor = GetTile(Position.X, Position.Y - 1);
		if (Neighbor->Paths.Contains(EPathDirection::Down) && !VisitedTiles.Contains(Neighbor))
		{
			if (HasPathToSource(Neighbor, VisitedTiles))
			{
				return true;
			}
		}
	}
	if (Tile->Paths.Contains(EPathDirection::Right)
		&& Tile->PuzzlePosition.X < MaxColumns - 1)
	{
		ARotatableTile* Neighbor = GetTile(Position.X + 1, Position.Y);
		if (Neighbor->Paths.Contains(EPathDirection::Left) && !VisitedTiles.Contains(Neighbor))
		{
			if (HasPathToSource(Neighbor, VisitedTiles))
			{
				return true;
			}
		}
	}
	if (Tile->Paths.Contains(EPathDirection::Down)
		&& Tile->PuzzlePosition.Y < MaxRows - 1)
	{
		ARotatableTile* Neighbor = GetTile(Position.X, Position.Y + 1);
		if (Neighbor->Paths.Contains(EPathDirection::Up) && !VisitedTiles.Contains(Neighbor))
		{
			if (HasPathToSource(Neighbor, VisitedTiles))
			{
				return true;
			}
		}
	}
	return false;
}

bool ARotationPuzzle::IsComplete()
{
	ARotatableTile* LastTile = GetTile(MaxColumns - 1, MaxRows - 1);
	return LastTile->Paths.Contains(EPathDirection::Right) && HasPathToSource(LastTile);
}


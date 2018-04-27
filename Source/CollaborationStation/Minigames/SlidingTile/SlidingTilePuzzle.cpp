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
#include "SlidingTilePuzzle.h"
#include "SlidingTile.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlidingTilePuzzle::ASlidingTilePuzzle() :
	bHasGap(false)
{
	PrimaryActorTick.bCanEverTick = false;
	NeighborOffsets.Add(FIntPoint(-1, 0));
	NeighborOffsets.Add(FIntPoint(1, 0));
	NeighborOffsets.Add(FIntPoint(0, -1));
	NeighborOffsets.Add(FIntPoint(0, 1));
}

// Called when the game starts or when spawned
void ASlidingTilePuzzle::BeginPlay()
{
	if (TileClass)
	{
		for (int32 Row = 0; Row < 4; Row++)
		{
			for (int32 Column = 0; Column < 4; Column++)
			{
				FTransform Transform;
				ASlidingTile* Tile = GetWorld()->SpawnActorDeferred<ASlidingTile>(TileClass, Transform, this);
				Tile->StartPosition = FIntPoint(Column, Row);
				Tile->Position = Tile->StartPosition;
				Tile->Puzzle = this;
				Tile->Material = UMaterialInstanceDynamic::Create(Material, this);
				Tile->Material->SetScalarParameterValue(FName(TEXT("Column")), Column);
				Tile->Material->SetScalarParameterValue(FName(TEXT("Row")), Row);
				Tiles.Add(Tile);
				UGameplayStatics::FinishSpawningActor(Tile, Transform);
			}
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TileClass not specified"));
	}
	Super::BeginPlay();
}

bool ASlidingTilePuzzle::RemoveTileToCreateGap(ASlidingTile* Tile)
{
	if (!Tiles.Contains(Tile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Tile not in puzzle!"));
		return false;
	}
	else if (bHasGap)
	{
		UE_LOG(LogTemp, Warning, TEXT("This puzzle already has a gap at %d,%d. Ignoring this call."), Gap.X, Gap.Y);
		return false;
	}
	else
	{
		bHasGap = true;
		Tiles.Remove(Tile);
		Gap = Tile->Position;
		return true;
	}
}

bool ASlidingTilePuzzle::ReinsertGapTile(ASlidingTile * Tile)
{
	if (Tile)
	{
		Tiles.Add(Tile);
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ReinsertGapTile: Tile is null"));
		return false;
	}
}

bool ASlidingTilePuzzle::CanMove(ASlidingTile * Tile)
{
	if (!Tiles.Contains(Tile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Tile not in puzzle!"));
		return false;
	}
	if (!bHasGap)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no gap yet!"));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attempt to move from %d,%d to %d,%d"), Tile->Position.X, Tile->Position.Y, Gap.X, Gap.Y);
	const int32 horizontalDifference = abs(Tile->Position.X - Gap.X);
	const int32 verticalDifference = abs(Tile->Position.Y - Gap.Y);
	return (horizontalDifference == 1 && verticalDifference == 0)
		|| (horizontalDifference == 0 && verticalDifference == 1);
}

bool ASlidingTilePuzzle::Move(ASlidingTile * Tile)
{
	if (!Tiles.Contains(Tile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Tile not in puzzle!"));
		return false;
	}
	if (CanMove(Tile))
	{
		if (SlidingSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SlidingSound);
		}
		FIntPoint temp = Tile->Position;
		Tile->Position = Gap;
		Gap = temp;
		OnTileMoved(Tile, Gap, Tile->Position);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Call CanMove first to ensure this call is legal."));
		return false;
	}
}

bool ASlidingTilePuzzle::IsSolved()
{
	for (const auto &Tile : Tiles)
	{
		if (Tile->Position != Tile->StartPosition)
		{
			return false;
		}
	}
	return true;
}

ASlidingTile * ASlidingTilePuzzle::GetRandomMovableTile()
{
	if (!bHasGap)
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no movable tiles because there is no gap"));
		return nullptr;
	}
	else
	{
		TArray<FIntPoint> MovableTileCoords;
		for (auto Offset : NeighborOffsets)
		{
			FIntPoint Point = Gap + Offset;
			if (IsOnMap(Point))
			{
				MovableTileCoords.Add(Point);
			}
		}
		ASlidingTile* Tile;
		do {
			int32 Index = FMath::RandRange(0, MovableTileCoords.Num() - 1);
			Tile = GetTileAt(MovableTileCoords[Index]);
		} while (Tile == PreviousRandomTile);
		PreviousRandomTile = Tile;
		return Tile;
	}
}

bool ASlidingTilePuzzle::IsOnMap(FIntPoint Point)
{
	return Point.X >= 0 && Point.X < 4 && Point.Y >= 0 && Point.Y < 4;
}

ASlidingTile * ASlidingTilePuzzle::GetTileAt(FIntPoint Point)
{
	for (int i = 0; i < Tiles.Num(); i++)
	{
		if (Tiles[i]->Position == Point)
		{
			return Tiles[i];
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No tile found for point %d,%d"), Point.X, Point.Y);
	return nullptr;
}


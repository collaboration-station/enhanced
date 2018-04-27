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
#include "RotatableTile.h"
#include "RotationPuzzle.h"

// Sets default values
ARotatableTile::ARotatableTile() :
	bGlowing(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotatableTile::BeginPlay()
{
	FTypeAndRotation TAndR = ComputeTypeAndOrientation(Paths);
	Type = TAndR.Type;
	Orientation = TAndR.Orientation;
	Super::BeginPlay();
}

void ARotatableTile::Turn()
{
	TSet<EPathDirection> NewPaths;
	if (Paths.Contains(EPathDirection::Up))
	{
		NewPaths.Add(EPathDirection::Right);
	}
	if (Paths.Contains(EPathDirection::Right))
	{
		NewPaths.Add(EPathDirection::Down);
	}
	if (Paths.Contains(EPathDirection::Down))
	{
		NewPaths.Add(EPathDirection::Left);
	}
	if (Paths.Contains(EPathDirection::Left))
	{
		NewPaths.Add(EPathDirection::Up);
	}
	Paths = NewPaths;

	// Turning any tile could make others' connection status change,
	// so update all the tiles.
	if (Puzzle)
	{
		Puzzle->UpdateTiles();
	}
}

bool ARotatableTile::UpdateGlowStatus()
{
	if (Puzzle)
	{
		bool bHasPathToSource = Puzzle->HasPathToSource(this);
		if (bHasPathToSource != bGlowing)
		{
			bGlowing = bHasPathToSource;
			OnGlowChange(bHasPathToSource);
		}
	}
	return bGlowing;
}

// Called every frame
void ARotatableTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTypeAndRotation ARotatableTile::ComputeTypeAndOrientation(TSet<EPathDirection> Paths)
{
	if (Paths.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rotatable tile created with %d paths"), Paths.Num());
		return FTypeAndRotation();
	}
	else if (Paths.Num() == 2)
	{
		if (Paths.Contains(EPathDirection::Up) && Paths.Contains(EPathDirection::Down))
		{
			return FTypeAndRotation(ETileType::Type_I, 0);
		}
		else if (Paths.Contains(EPathDirection::Left) && Paths.Contains(EPathDirection::Right))
		{
			return FTypeAndRotation(ETileType::Type_I, 90);
		}
		else if (Paths.Contains(EPathDirection::Up))
		{
			return FTypeAndRotation(ETileType::Type_L, Paths.Contains(EPathDirection::Left) ? - 90 : 0);
		}
		else
		{
			// L with a downward path
			return FTypeAndRotation(ETileType::Type_L, Paths.Contains(EPathDirection::Left) ? 180 : 90);
		}
	}
	else
	{
		if (Paths.Num() != 3)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rotatable tile created with 4 paths"));
			return FTypeAndRotation();
		}
		if (!Paths.Contains(EPathDirection::Up))
		{
			return FTypeAndRotation(ETileType::Type_T, 0);
		}
		else if (!Paths.Contains(EPathDirection::Right))
		{
			return FTypeAndRotation(ETileType::Type_T, 90);
		}
		else if (!Paths.Contains(EPathDirection::Down))
		{
			return FTypeAndRotation(ETileType::Type_T, 180);
		}
		else
		{
			return FTypeAndRotation(ETileType::Type_T, -90);
		}
	}
}


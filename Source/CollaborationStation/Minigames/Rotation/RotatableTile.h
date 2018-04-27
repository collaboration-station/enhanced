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
#include "RotatableTile.generated.h"

// The possible exit directions for a path on a rotatable tile.
UENUM(BlueprintType)
enum class EPathDirection : uint8
{
	Up, Left, Down, Right
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	// Two paths at a right angle
	Type_L,
	// Two paths in a straight line
	Type_I,
	// Three paths in a "T" formation
	Type_T
};

struct FTypeAndRotation {
	ETileType Type;
	// Rotation of the canonical form, in degrees
	float Orientation;

	FTypeAndRotation() : Type(ETileType::Type_I), Orientation(0) {}
	FTypeAndRotation(ETileType type, float rotation) : Type(type), Orientation(rotation) {}
};

UCLASS()
class COLLABORATIONSTATION_API ARotatableTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatableTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The type of tile that this is
	UPROPERTY(BlueprintReadOnly)
	ETileType Type;

	// The orientation of tile with respect to the canonical form.
	// This is used during object construction.
	UPROPERTY(BlueprintReadOnly)
	float Orientation;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGlowChange(bool Glowing);

	UPROPERTY(BlueprintReadOnly)
	bool bGlowing;

public:	
	// Turn the tile (model-layer) 90 degrees clockwise.
	UFUNCTION(BlueprintCallable)
	void Turn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The ways out of this tile.
	// These are set by the puzzle when it creates the tiles.
	UPROPERTY(BlueprintReadOnly)
	TSet<EPathDirection> Paths;

	// The puzzle of which this is a part
	UPROPERTY(BlueprintReadOnly)
	class ARotationPuzzle* Puzzle;

	// The coordinates of this tile within its puzzle
	FIntPoint PuzzlePosition;

	// Check if this tile has a path to the source and update its glow accordingly.
	// Returns the new status.
	bool UpdateGlowStatus();
	
private:
	// Given tile paths, determine which type this is.
	static FTypeAndRotation ComputeTypeAndOrientation(TSet<EPathDirection> Paths);
};

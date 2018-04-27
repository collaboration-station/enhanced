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
#include "Grid.generated.h"

UCLASS()
class COLLABORATIONSTATION_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	// Array of all possible cards (one of each), which can be spawned in pairs
	// into the grid.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInstanceConstant*> CardLibrary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACard> CardToSpawn;

	// Sound to play when a match is made
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* MatchSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Rows = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Columns = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShuffle;

	// Sets default values for this actor's properties
	AGrid();

	// Called when a card is touched
	UFUNCTION()
	void CardPress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	// Call to notify the grid that a card's animation has completed
	void OnCardFlipComplete(ACard* Card);

	// Find and make a match between two facedown cards, as when cheating
	void ForceMatch();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ACard*> Cards;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGridCompleteEffect();

	// Play the effect of mismatching these two cards
	UFUNCTION(BlueprintImplementableEvent)
	void PlayMismatchEffect(ACard* Card1, ACard* Card2);

	// Called by the blueprints to notify that the effect is complete
	UFUNCTION(BlueprintCallable)
	void OnPlayMismatchEffectComplete();

private:
	FVector GetLocationFromGridLocation(const int32 x, const int32 y) const;

	FRandomStream RandomStream;

	TArray<ACard*> ActiveCards;
	TArray<ACard*> ForcedMatchCards;

	uint32 RemainingMatches;

	// Call when a match is made to determine if this board is complete.
	void OnMatchMade();

	// Indicates if we are currently in a mismatch delay, which means we should not handle new input at the time.
	bool bInMismatchDelay;

	// Called when the mismatch delay is over
	void OnMismatchDelayComplete();
};

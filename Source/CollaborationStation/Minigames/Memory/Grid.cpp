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
#include "Grid.h"
#include "Card.h"
#include "../MinigamePlayerController.h"
#include "MemoryGameMode.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Sets default values
AGrid::AGrid() :
	bShuffle(1),
	bInMismatchDelay(false)
{
	PrimaryActorTick.bCanEverTick = false;

	// Random seed
	int64 DateInSeconds = FDateTime::Now().ToUnixTimestamp();
	RandomStream = FRandomStream();
	RandomStream.Initialize(DateInSeconds);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	ensureMsgf(Rows*Columns == CardLibrary.Num() * 2, TEXT("There should be the right number of card images to fill the grid."));
	RemainingMatches = Rows * Columns / 2;

	// Make a deck containing two of each image
	TArray<UMaterialInstanceConstant*> Deck;
	for (auto& Image : CardLibrary) {
		Deck.Add(Image);
		Deck.Add(Image);
	}

	// Shuffle the deck
	if (bShuffle)
	{
		for (int32 i = Deck.Num() - 1; i > 0; i--) {
			int32 j = FMath::FloorToInt(RandomStream.FRand() * (i + 1)) % Deck.Num();
			auto temp = Deck[i];
			Deck[i] = Deck[j];
			Deck[j] = temp;
		}
	}

	// Spawn the cards
	uint32 ImageIndex = 0;
	UWorld* const World = GetWorld();
	if (World) 
	{
		for (int row = 0; row < Rows; row++) {
			for (int col = 0; col < Columns; col++) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FRotator NoSpawnRotation(0.0f, 0.0f, 0.0f);
				FVector SpawnLocation = GetLocationFromGridLocation(col, row);

				ACard* Card = World->SpawnActor<ACard>(CardToSpawn, SpawnLocation, NoSpawnRotation, SpawnParams);
				Card->CardFront = Deck[ImageIndex++];
				Card->Grid = this;
				Card->OnInputTouchBegin.AddUniqueDynamic(this, &AGrid::CardPress);
				Cards.Add(Card);
			}
		}
	}
}

void AGrid::OnPlayMismatchEffectComplete()
{
	ActiveCards[0]->Flip();
	ActiveCards[1]->Flip();
	ActiveCards.Empty(2);
	bInMismatchDelay = false;
}

FVector AGrid::GetLocationFromGridLocation(const int32 x, const int32 y) const
{
	int32 CardWidth = GridWidth / Columns;
	int32 CardHeight = GridHeight / Rows;
	FVector Center = GetActorLocation();
	FVector Result = FVector(x * CardWidth + CardWidth / 2, 0, y * CardHeight + CardHeight / 2);
	Result.X -= GridWidth / 2;
	Result.Z -= GridHeight / 2;
	Result += Center;
	return Result;
}


void AGrid::CardPress(ETouchIndex::Type FingerIndex, AActor * TouchedActor)
{
	ACard* TouchedCard = Cast<ACard>(TouchedActor);

	// If it's already flipping or matched or we're in a mismatch delay, ignore the touch.
	if (TouchedCard->IsFlipping() || TouchedCard->IsMatched() || bInMismatchDelay)
	{
		return;
	}

	// If it's face up, flip it over and make it inactive
	else if (TouchedCard->GetState() == MemoryCard::FaceUp)
	{
		check(ActiveCards.Contains(TouchedCard));
		ActiveCards.Remove(TouchedCard);
		TouchedCard->Flip();
	}

	// Otherwise it was face down, so flip it up if we don't have two active cards
	else if (ActiveCards.Num() < 2)
	{
		check(TouchedCard->GetState() == MemoryCard::FaceDown);
		ActiveCards.Add(TouchedCard);
		TouchedCard->Flip();
	}
}

void AGrid::ForceMatch()
{
	check(ForcedMatchCards.Num() == 0); // Can only force one match at a time.
	for (auto Card1 : Cards)
	{
		if (Card1->GetState() == MemoryCard::FaceDown)
		{
			for (auto Card2 : Cards)
			{
				if (Card1 == Card2)
				{
					continue;
				}
				else if (Card2->GetState() == MemoryCard::FaceDown
					&& Card1->Matches(Card2))
				{
					// Found a match of facedown cards. Flip 'em up.
					ForcedMatchCards.Add(Card1);
					ForcedMatchCards.Add(Card2);
					Card1->Flip();
					Card2->Flip();
					return;
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Could not find two face-down cards to match"));
}

void AGrid::OnCardFlipComplete(ACard* Card) 
{
	// Did the card flip down? We don't care about it.
	if (Card->GetState()==MemoryCard::FaceDown)
	{
		return;
	}

	// Was this a forced match?
	if (ForcedMatchCards.Contains(Card))
	{
		// Are both face-up now?
		if (ForcedMatchCards[0]->GetState() == MemoryCard::FaceUp
			&& ForcedMatchCards[1]->GetState() == MemoryCard::FaceUp)
		{
			// Finish the forced match.
			ForcedMatchCards[0]->SetMatched();
			ForcedMatchCards[1]->SetMatched();
			ForcedMatchCards.Empty();

			OnMatchMade();
		}
	}

	// Otherwise, are there two face up cards?
	else if (ActiveCards.Num() == 2
		&& ActiveCards[0]->GetState() == MemoryCard::FaceUp
		&& ActiveCards[1]->GetState() == MemoryCard::FaceUp)
	{
		// Is it a match?
		if (ActiveCards[0]->Matches(ActiveCards[1]))
		{
			ActiveCards[0]->SetMatched();
			ActiveCards[1]->SetMatched();
			ActiveCards.Empty();

			OnMatchMade();
		}
		else
		{
			bInMismatchDelay = true;
			PlayMismatchEffect(ActiveCards[0], ActiveCards[1]);
		}
	}
}

void AGrid::OnMatchMade()
{
	if (MatchSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), MatchSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Match sound not specified"));
	}
	
	AMemoryGameMode* GameMode = Cast<AMemoryGameMode>(GetWorld()->GetAuthGameMode());
	checkf(GameMode, TEXT("Game mode cast failed"));
	if (GameMode)
	{
		GameMode->AddPoints(GameMode->PointsPerMatch);
	}

	RemainingMatches--;
	if (RemainingMatches == 0)
	{
		// Signal the animation to play
		PlayGridCompleteEffect();
	}
}

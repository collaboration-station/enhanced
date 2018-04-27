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
#include "Card.h"
#include "Grid.h"



// Sets default values
ACard::ACard() : bIsMatched(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACard::Flip()
{
	switch (State) {
	case MemoryCard::FaceDown:
		State = MemoryCard::FlippingUp;
		PlayFlipEffect();
		break;
	case MemoryCard::FaceUp:
		State = MemoryCard::FlippingDown;
		PlayFlipEffect();
		break;
	default:
		checkNoEntry();
	}
}

void ACard::OnFlipEffectComplete() {
	switch (State)
	{
	case MemoryCard::FlippingUp:
		State = MemoryCard::FaceUp;
		break;
	case MemoryCard::FlippingDown:
		State = MemoryCard::FaceDown;
		break;
	default:
		checkNoEntry();
	}
	if (Grid)
	{
		Grid->OnCardFlipComplete(this);
	}
}

void ACard::SetMatched() {
	bIsMatched = true;
	PlayMatchedEffect();
}
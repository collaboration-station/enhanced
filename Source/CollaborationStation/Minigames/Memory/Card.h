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
#include "Card.generated.h"

class UMaterialInstanceConstant;

namespace MemoryCard {
	enum EState {
		FaceDown, FlippingUp, FaceUp, FlippingDown
	};
}

UCLASS()
class COLLABORATIONSTATION_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ExposeOnSpawn=true))
	UMaterialInstanceConstant* CardFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceConstant* CardBack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AGrid* Grid;

	// Flip the card over.
	UFUNCTION(BlueprintCallable)
	void Flip();

	// Call when the flip effect is done.
	UFUNCTION(BlueprintCallable)
	void OnFlipEffectComplete();

	inline bool IsMatched() { return bIsMatched;  }
	void SetMatched();

	inline MemoryCard::EState GetState() { return State; }

	inline bool IsFlipping() { return State == MemoryCard::FlippingDown || State == MemoryCard::FlippingUp;  }

	// Check if this card matches the other one
	inline bool Matches(const ACard* Other) const {
		return (Other && CardFront == Other->CardFront);
	}

protected:
	// Called to initiate the matched effect
	UFUNCTION(BlueprintImplementableEvent)
	void PlayMatchedEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFlipEffect();

private:
	// Indicates if this has been successfully matched
	uint32 bIsMatched : 1;

	MemoryCard::EState State = MemoryCard::FaceDown;
};

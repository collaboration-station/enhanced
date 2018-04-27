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
#include "SlidingTileGameMode.h"
#include "PaperSprite.h"

TArray<UPaperSprite*> ASlidingTileGameMode::LoadTiles(FString PathPrefix)
{
	TArray<UPaperSprite*> Array;
	for (int i = 0; i < 16; i++)
	{
		FString Path = *PathPrefix + FString::FormatAsNumber(i);
		UE_LOG(LogTemp, Warning, TEXT("Loading path %s"), *Path);
		UObject* something = StaticLoadObject(UObject::StaticClass(), nullptr, *Path);
		if (something)
		{
			UPaperSprite* bp = Cast<UPaperSprite>(something);
			if (bp)
			{
				UE_LOG(LogTemp, Warning, TEXT("Got it"));
				Array.Add(bp);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("bp is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("something is null"));
		}
	}
	return Array;
}
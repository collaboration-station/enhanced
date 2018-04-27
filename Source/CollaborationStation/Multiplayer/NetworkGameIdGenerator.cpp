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
#include "NetworkGameIdGenerator.h"

FNetworkGameId UNetworkGameIdGenerator::GenerateNetworkGameId()
{
	check(Adjectives.Num() > 0 && "Adjectives were not loaded from config file");
	check(Nouns.Num() > 0 && "Nouns were not loaded from config file");
	int32 AdjectiveIndex = FMath::Rand() % Adjectives.Num();
	int32 NounIndex = FMath::Rand() % Nouns.Num();
	int32 Number = FMath::Rand() % (MaxMissionNumber - MinMissionNumber + 1) + MinMissionNumber;
	FNetworkGameId Result(Adjectives[AdjectiveIndex], Nouns[NounIndex], Number);
	return Result;
}

FString UNetworkGameIdGenerator::NetworkGameIdToString(FNetworkGameId Code)
{
	return FString::Printf(TEXT("%s%s%d"), *Code.Adjective, *Code.Noun, Code.Number);
}

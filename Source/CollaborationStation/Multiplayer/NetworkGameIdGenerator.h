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
#include "UObject/NoExportTypes.h"
#include "NetworkGameIdGenerator.generated.h"

USTRUCT(BlueprintType)
struct FNetworkGameId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Adjective;

	UPROPERTY(BlueprintReadOnly)
	FString Noun;

	UPROPERTY(BlueprintReadOnly)
	int32 Number;
	
	FNetworkGameId() {}

	FNetworkGameId(FString adjective, FString noun, uint32 number) : Adjective(adjective), Noun(noun), Number(number) {}
};

/**
 * 
 */
UCLASS(config=Game, Blueprintable)
class COLLABORATIONSTATION_API UNetworkGameIdGenerator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, BlueprintReadOnly)
	TArray<FString> Adjectives;
	
	UPROPERTY(Config, BlueprintReadOnly)
	TArray<FString> Nouns;

	UPROPERTY(Config, BlueprintReadOnly)
	int32 MinMissionNumber;

	UPROPERTY(Config, BlueprintReadOnly)
	int32 MaxMissionNumber;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FNetworkGameId GenerateNetworkGameId();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString NetworkGameIdToString(FNetworkGameId Code);
	
};

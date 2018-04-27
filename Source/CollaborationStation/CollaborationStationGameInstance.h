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

#include "Multiplayer/NetworkFacade.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "Engine/DataAsset.h"
#include "CollaborationStationGameInstance.generated.h"

// Country-of-origin identifier for crew members
UENUM(BlueprintType)
enum class ECountry : uint8
{
	Canada,
	China,
	EU,
	Russia,
	USA
};

USTRUCT(BlueprintType)
struct FMinigameInfo
{
	GENERATED_BODY()

	// The name of the minigame as it can be loaded from the filesystem
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LevelName;

	// String identifier for this minigame; used in network protocol.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NetworkId;

	// The name of the minigame as shown to the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	// A short description of the minigame as could be shown in the task selection widget.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Description;

	FMinigameInfo() {}
};

UCLASS()
class COLLABORATIONSTATION_API UMinigameRegistry : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMinigameInfo> Minigames;
};

class UMultiplayerGameGoals;

/**
 * Game instance for Collaboration Station
 */
UCLASS(config=Game)
class COLLABORATIONSTATION_API UCollaborationStationGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

	UCollaborationStationGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	// The available minigames
	// 
	// Want to hear something really weird? This fails if I call the property just "Minigames".
	// If I call it something with two words, like "AvailableMinigames" or "RegisteredMinigames",
	// then it works. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMinigameRegistry* MinigameRegistry;

	// Configuration for the goals and length of play
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMultiplayerGameGoals* MultiplayerGameGoals;

	// Get the minigame info array from the registry (convenience method)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline TArray<FMinigameInfo> GetRegisteredMinigames() { check(MinigameRegistry && "No minigame registry!"); return MinigameRegistry->Minigames; };

	// Indicates if we are in training mode or a real expedition.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Configuration")
	uint32 bTraining : 1;

	// The local player's country code
	UPROPERTY(BlueprintReadWrite,EditInstanceOnly)
	ECountry Country;

	// Is the local machine's music currently muted.
	UPROPERTY(BlueprintReadWrite, Category = Audio)
	bool bMuteMusic;
};

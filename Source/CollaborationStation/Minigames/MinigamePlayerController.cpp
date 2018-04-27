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
#include "MinigamePlayerController.h"
#include "ConstructorHelpers.h"
#include "UserWidget.h"

AMinigamePlayerController::AMinigamePlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BoostUIFinder(TEXT("/Game/Minigames/Common/Widget_Common"));
	if (BoostUIFinder.Succeeded()) {
		CommonUiClass = BoostUIFinder.Class;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Locating common UI class failed"));
	}
	bShowMouseCursor = true;
}

void AMinigamePlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
	if (!CommonUiWidget)
	{
		CommonUiWidget = CreateWidget<UUserWidget>(this, CommonUiClass);
		if (CommonUiWidget)
		{
			CommonUiWidget->AddToViewport();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Creation failed."));
		}
	}
	Super::BeginPlay();
}
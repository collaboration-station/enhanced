# Multiplayer Networking Protocol

## Data organization overview

This is a template of what Firebase could look like in the middle of a game.
Text in angular brackets would be replaced by actual values, or selected from
those separated by pipes (|). Ellipses (...) indicate repetition.
~~~~
lobby: {
  <code>: <generated id>,
  ...
}

games: {
  <generated id>: {
    created: <UTC timestamp>,
    code: <generated game code>,
    abort: <false|true>,
    state: <lobby|cancelled|brief|play|debrief|done>,
    players : {
      0: {
        uid: <player uid>
        country: <us|eu|ch|ru|ca>,
        round-1: {
            selection: <minigame>,
            score: <integer>
        },
        round-2: {
            selection: <minigame>,
            score: <integer>
        }
      },
      1: {
        uid: <player uid>
        country: <us|eu|ch|ru|ca>,
        round-1: {
            selection: <minigame>,
            score: <integer>
        },
        round-2: {
            selection: <minigame>,
            score: <integer>
        }
      }
    }
  }
}  
~~~~

## Protocol definition

General:
If someone gets disconnected, they should write 'true' to the game's `abort` field.
All other players can watch this field with a value listener to know that if it gets set,
we have to quit; doing anything else, like dropping a player and continuing, is out of scope.

Initiator (commander):
* pushes the following to `/games` to get the game id:
~~~
{
  created: <UtcTimestamp>,
  state: lobby,
  abort: false,
  players: {
    0: {
      uid: <playerUid>
    }
  }
}
~~~
* Generate a code.
* * Look up `/lobby/<code>`
* * If something is there already:
* * * If that game is `state`=="done", overwrite `/lobby/<code>` with this game id
* * * If that game is "live" (not "done" and not >1 hours old), generate new code, repeat.
* stores gameref in FirebaseFacade
* sets up listener to `gameref/players`, notifies number of players on screen
* Watch `state` for change to "brief"

TODO: Need to either have the clients or a standalone script occasionally close games by turning their `state` to `done`.

Commander can cancel the game by posting "cancelled" to `state`, prior to beginning the expedition (by posting `brief`).

Crew1:
* Query `lobby/<code>`
* * If there is a game there, make sure it is in `state`=="lobby", then update UI to allow join or cancel
* *  In all other cases, go back to entering a new code or to main menu

Crew1 joining:
* Transaction that puts the following to `<gameref>/players`.
~~~
[nextid]: {
  uid: <playerUid>
}
~~~
* stores gameref in game instance
* Watch `state` for change to "brief"

Commander starts the expedition:
* Automatically and randomly assigns country codes to each player
* Generate and post goals based on the # players
* Update `state` to "brief"
* Watch all players' `round-1/selection` 

For round _N_ from 1 to 3:

All players, on change of `state` to "brief"
* Store crew array in game instance (TODO: Could eliminate entirely, definitely don't repeat)
* Store player index in game instance  (TODO: Could eliminate entirely, definitely don't repeat)
* Store goals locally
* Note that players don't stop watching the state here; they need to know about future state changes.

Players select minigames and cofirm their selection
* Update their record `round-N/selection` to the minigame selected; this can happen multiple times.

Commander, watching all players' `round-N/selection`
* When a player selection is made, knock down a timer and remove that listener.
* When all have a selection, enable a "Start" button
* When started, 
* * update game `state` to "play"
* * watch each player's `round-N/score`

All players are still watching `state`, and it changes to "play"
* Load the selected minigames and play them.
* When done, post `round-N/score` to round-N in player structure
* * Score of 0 is failure; score of 1 is minimal success; score greater than that is bonus points

Commander, watching all players' `round-N/score`, when they all have one
* Remove `round-N/score` listener
* Update `state` to "debrief"

All players are still watching `state`, and it becomes "debrief":
* Update the local copy of a Progress data structure with round-N data
* Show results of the round
* Commander can advance to next briefing or conclusion if it's the last round

Commander advances to next round:
* Update `state` to "brief"

Commander advances to conclusion:
* Update `state` to "done"

All player see `state` change to "done":
* Players remove listeners from `state`
* Show game conclusion as computed from goals
* On exit, clear from game instance: game ref, player index, crew structure

## Error cases

TODO: Look into using a disconnection event to post something, like changing state to "dropout".
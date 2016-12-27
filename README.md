# Anti-Pause System by oMa37

Callbacks:

    OnPlayerPause(playerid, type);
    
    Types:
    1 - Player has tabbed out
    2 - Player is not moving
  
    OnPlayerUnPause(playerid, type, time);
    
    Types:
    1 - Player has disconnected
    1 - Player still connected
  
    Time in seconds
  
Functions:

    IsPlayerPaused(playerid);
    - Returns 1 if the player is paused, 0 if is not.
  
    GetPauseTime(playerid);
    - Returns player paused time in seconds.
  
    GetPauseType(playerid);
    - Returns 1 if the player tabbed out, 2 if the player is not moving.

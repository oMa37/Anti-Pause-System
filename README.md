# Anti-Pause System - Automatic


Callbacks:

    <color=red>OnPlayerPause(playerid, type);</color>
    
    Types:
    1 - Player has tabbed out
    2 - Player is not moving
  
    <color=red>OnPlayerUnPause(playerid, type, time);</color> 
    
    Types:
    1 - Player has disconnected
    2 - Player still connected
  
    Time in seconds
  
Functions:

    <color=red>IsPlayerPaused(playerid);</color> 
    - Returns 1 if the player is paused, 0 if is not.
  
    <color=red>GetPauseTime(playerid);</color> 
    - Returns player paused time in seconds.
  
    <color=red>GetPauseType(playerid);</color> 
    - Returns 1 if the player tabbed out, 2 if the player is not moving.

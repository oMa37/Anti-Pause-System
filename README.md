# Anti-Pause System - Automatic


Callbacks:

    OnPlayerPause(playerid, type);
    
    Types:
    1 - Player has tabbed out
    2 - Player is not moving
  
    OnPlayerUnPause(playerid, type, time);
    
    Types:
    1 - Player has disconnected
    2 - Player still connected
  
    Time in seconds
  
Functions:

    IsPlayerPaused(playerid);
    - Returns 1 if the player is paused, 0 if is not.
  
    GetPauseTime(playerid);
    - Returns player paused time in seconds.
  
    GetPauseType(playerid);
    - Returns 1 if the player tabbed out, 2 if the player is not moving.
    
Defines:

    #define PAUSE_LABEL
    - Enables the 3D text label on the player name.
    
    #define PAUSE_LABEL_TEXT "YourText"
    - Change the 3D label text.
    
    #define PAUSE_LABEL_COLOR 0xCOLOR
    - Change the 3D label color.
    
Example:
    
    #include <a_samp>
    
    #define PAUSE_LABEL
    #define PAUSE_LABEL_TEXT "I'M PAUSED PLAYER!"
    #define PAUSE_LABEL_COLOR 0xFF0000FF
    
    #include <AP>

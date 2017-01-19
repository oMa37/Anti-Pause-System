/*
	Anti-Pause System by oMa37

	Callbacks:

	OnPlayerPause(playerid, type);
	
	Types:
	1 - Player has tabbed out
	2 - Player is not moving

	OnPlayerUnPause(playerid, type, time);

	Types:
	1 - Player has disconnected
	2 - Player still connected

	Time in seconds.

	Functions:

	IsPlayerPaused(playerid);
	GetPauseTime(playerid);
	GetPauseType(playerid);

	Defines:

	#define PAUSE_LABEL - Enables the 3D text label on the player name.
    #define PAUSE_LABEL_TEXT "YourText" - Change the 3D label text.
    #define PAUSE_LABEL_COLOR (0xCOLOR) - Change the 3D label color.
*/

#if !defined INVALID_STREAMER_ID
	#error This include require streamer
#endif

#if ((defined PAUSE_LABEL) && (!defined PAUSE_LABEL_TEXT))
	#error Please define the 3D label text
#endif

#if ((defined PAUSE_LABEL) && (!defined PAUSE_LABEL_COLOR))
	#error Please define the 3D label color
#endif

// Variables

new
	TW_PAUSE_TICK 			[MAX_PLAYERS],
	TW_PAUSE_TIME 			[MAX_PLAYERS],
	TW_PAUSE_TYPE 			[MAX_PLAYERS],
	TW_MOVE_TIME 			[MAX_PLAYERS],
	Float:TW_PAUSE_LASTX 	[MAX_PLAYERS],
	Float:TW_PAUSE_LASTY 	[MAX_PLAYERS],
	Float:TW_PAUSE_LASTZ 	[MAX_PLAYERS],
	Text3D:TW_PAUSE_LABEL 	[MAX_PLAYERS],
	TW_ISPAUSED 			[MAX_PLAYERS],
	TW_PAUSE_CHECK_TIMER;


// Defines

#define UNPAUSE_TYPE_DISCONNECTED 1
#define UNPAUSE_TYPE_CONNECTED 2

#define PAUSE_TYPE_TABOUT 1
#define PAUSE_TYPE_MOVE 2

#define IsPlayerPaused(%0) TW_ISPAUSED[(%0)]
#define GetPauseTime(%0) gettime() - TW_PAUSE_TIME[(%0)]
#define GetPauseType(%0) TW_PAUSE_TYPE[(%0)]


// Callbacks

#if defined FILTERSCRIPT

	public OnFilterScriptInit() {
		
		TW_PAUSE_CHECK_TIMER = SetTimer("PausedCheck", 5000, true);

		#if defined TW_OnFilterScriptInit
			return TW_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}

	public OnFilterScriptExit() {
		
		DestroyAllDynamic3DTextLabels();
		KillTimer(TW_PAUSE_CHECK_TIMER);

		#if defined TW_OnFilterScriptExit
			return TW_OnFilterScriptExit();
		#else
			return 1;
		#endif
	}

#else

	public OnGameModeInit() {

		TW_PAUSE_CHECK_TIMER = SetTimer("PausedCheck", 5000, true);

		#if defined TW_OnGameModeInit
			return TW_OnGameModeInit();
		#else
			return 1;
		#endif
	}

	public OnGameModeExit() {

		DestroyAllDynamic3DTextLabels();
		KillTimer(TW_PAUSE_CHECK_TIMER);

		#if defined TW_OnGameModeExit
			return TW_OnGameModeExit();
		#else
			return 1;
		#endif
	}

#endif

public OnPlayerConnect(playerid) {

	if(IsValidDynamic3DTextLabel(TW_PAUSE_LABEL[playerid]))
		DestroyDynamic3DTextLabel(TW_PAUSE_LABEL[playerid]);

	TW_PAUSE_TICK[playerid] = 0;
	TW_PAUSE_TIME[playerid] = 0;
	TW_ISPAUSED[playerid] = 0;

	#if defined TW_OnPlayerConnect
		return TW_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerDisconnect(playerid, reason) {

	if(IsValidDynamic3DTextLabel(TW_PAUSE_LABEL[playerid]))
		DestroyDynamic3DTextLabel(TW_PAUSE_LABEL[playerid]);

	TW_PAUSE_TICK[playerid] = 0;
	TW_PAUSE_TIME[playerid] = 0;
	TW_ISPAUSED[playerid] = 0;

	CallLocalFunction("OnPlayerUnPause", "ii", playerid, UNPAUSE_TYPE_DISCONNECTED);

	#if defined TW_OnPlayerDisconnect
		return TW_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}

public OnPlayerUpdate(playerid) {

	TW_PAUSE_TICK[playerid] = gettime();
	GetPlayerPos(playerid, TW_PAUSE_LASTX[playerid], TW_PAUSE_LASTY[playerid], TW_PAUSE_LASTZ[playerid]);

	#if defined TW_OnPlayerUpdate
		return TW_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerText(playerid, text[]) {
	
	TW_MOVE_TIME[playerid] = gettime();

	#if defined TW_OnPlayerText
		return TW_OnPlayerText(playerid, text);
	#else
		return 1;
	#endif
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {

	TW_MOVE_TIME[playerid] = gettime();

	#if defined TW_OnPlayerKeyStateChange
		return TW_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}

public PausedCheck() {

	for(new i, j = GetPlayerPoolSize(); i <= j; i++) {

		if(IsPlayerConnected(i)) {

			new Float:x, Float:y, Float:z;
			GetPlayerPos(i, x, y, z);

			if(gettime() - TW_PAUSE_TICK[i] < 4 && TW_ISPAUSED[i] == 0) {

				TW_ISPAUSED[i] = 1;
				TW_PAUSE_TIME[i] = gettime();
				TW_PAUSE_TYPE[i] = PAUSE_TYPE_TABOUT;

				#if ((defined PAUSE_LABEL) && (defined PAUSE_LABEL_TEXT) && (defined PAUSE_LABEL_COLOR))
					TW_PAUSE_LABEL[i] = CreateDynamic3DTextLabel(PAUSE_LABEL_TEXT, PAUSE_LABEL_COLOR, x, y, z+0.9, 15.0, i);
				#endif

				CallLocalFunction("OnPlayerPause", "ii", i, PAUSE_TYPE_TABOUT);
			}
			else if((TW_PAUSE_LASTX[i] == x && TW_PAUSE_LASTY[i] == y && TW_PAUSE_LASTZ[i] == z) && gettime() - TW_MOVE_TIME[i] < 6 && TW_ISPAUSED[i] == 0) {

				TW_ISPAUSED[i] = 1;
				TW_PAUSE_TIME[i] = gettime();
				TW_MOVE_TIME[i] = gettime();
				TW_PAUSE_TYPE[i] = PAUSE_TYPE_MOVE;

				#if ((defined PAUSE_LABEL) && (defined PAUSE_LABEL_TEXT) && (defined PAUSE_LABEL_COLOR))
					TW_PAUSE_LABEL[i] = CreateDynamic3DTextLabel(PAUSE_LABEL_TEXT, PAUSE_LABEL_COLOR, x, y, z+0.9, 15.0, i);
				#endif

				CallLocalFunction("OnPlayerPause", "ii", i, PAUSE_TYPE_MOVE);
			}
			else {

				TW_ISPAUSED[i] = 0;

				if(IsValidDynamic3DTextLabel(TW_PAUSE_LABEL[i]))
					DestroyDynamic3DTextLabel(TW_PAUSE_LABEL[i]);

				CallLocalFunction("OnPlayerUnPause", "iii", i, UNPAUSE_TYPE_CONNECTED, GetPauseTime(i));
			}
		}
	}
}


// Hooks

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit TW_OnFilterScriptInit
#if defined TW_OnFilterScriptInit
	forward TW_OnFilterScriptInit();
#endif

#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif

#define OnFilterScriptExit TW_OnFilterScriptExit
#if defined TW_OnFilterScriptExit
	forward TW_OnFilterScriptExit();
#endif

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit TW_OnGameModeInit
#if defined TW_OnGameModeInit
	forward TW_OnGameModeInit();
#endif

#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif

#define OnGameModeExit TW_OnGameModeExit
#if defined TW_OnGameModeExit
	forward TW_OnGameModeExit();
#endif

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect TW_OnPlayerConnect
#if defined TW_OnPlayerConnect
	forward TW_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect TW_OnPlayerDisconnect
#if defined TW_OnPlayerDisconnect
	forward TW_OnPlayerDisconnect(playerid);
#endif

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate TW_OnPlayerUpdate
#if defined TW_OnPlayerUpdate
	forward TW_OnPlayerUpdate(playerid);
#endif

#if defined _ALS_OnPlayerText
	#undef OnPlayerText
#else
	#define _ALS_OnPlayerText
#endif

#define OnPlayerText TW_OnPlayerText
#if defined TW_OnPlayerText
	forward TW_OnPlayerText(playerid, text[]);
#endif

#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange TW_OnPlayerKeyStateChange
#if defined TW_OnPlayerKeyStateChange
	forward TW_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

// Forwards

forward PausedCheck();
forward OnPlayerPause(playerid, type);
forward OnPlayerUnPause(playerid, type, time);

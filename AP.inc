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

	#define PAUSE_LABEL - This will enable the 3D text label on the player name (Text: AFK)
*/

#if !defined INVALID_STREAMER_ID
	#error This include require streamer include
#endif


// Variables

new
	pTick 			[MAX_PLAYERS],
	pTime 			[MAX_PLAYERS],
	pType 			[MAX_PLAYERS],
	pMoveTime 		[MAX_PLAYERS],
	Float:pLastX 	[MAX_PLAYERS],
	Float:pLastY 	[MAX_PLAYERS],
	Float:pLastZ 	[MAX_PLAYERS],
	Text3D:pLabel 	[MAX_PLAYERS],
	IsPaused 		[MAX_PLAYERS];


// Defines

#define UNPAUSE_TYPE_DISCONNECTED 1
#define UNPAUSE_TYPE_CONNECTED 2

#define PAUSE_TYPE_TABOUT 1
#define PAUSE_TYPE_MOVE 2

#define IsPlayerPaused(%0) IsPaused[(%0)]
#define GetPauseTime(%0) gettime() - pTime[(%0)]
#define GetPauseType(%0) pType[(%0)]


// Callbacks

#if defined FILTERSCRIPT

	public OnFilterScriptExit() {
		
		DestroyAllDynamic3DTextLabels();

		#if defined TW_OnFilterScriptExit
			return TW_OnFilterScriptExit();
		#else
			return 1;
		#endif
	}

	public OnFilterScriptInit() {
		
		SetTimer("PausedCheck", 5000, true);

		#if defined TW_OnFilterScriptInit
			return TW_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}

#endif	

public OnGameModeInit() {

	SetTimer("PausedCheck", 5000, true);

	#if defined TW_OnGameModeInit
		return TW_OnGameModeInit();
	#else
		return 1;
	#endif
}

public OnGameModeExit() {

	DestroyAllDynamic3DTextLabels();

	#if defined TW_OnGameModeExit
		return TW_OnGameModeExit();
	#else
		return 1;
	#endif
}

public OnPlayerConnect(playerid) {

	if(IsValidDynamic3DTextLabel(pLabel[playerid]))
		DestroyDynamic3DTextLabel(pLabel[playerid]);

	pTick[playerid] = 0;
	pTime[playerid] = 0;
	IsPaused[playerid] = 0;

	#if defined TW_OnPlayerConnect
		return TW_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerDisconnect(playerid) {

	if(IsValidDynamic3DTextLabel(pLabel[playerid]))
		DestroyDynamic3DTextLabel(pLabel[playerid]);

	pTick[playerid] = 0;
	pTime[playerid] = 0;
	IsPaused[playerid] = 0;

	CallLocalFunction("OnPlayerUnPause", "ii", playerid, UNPAUSE_TYPE_DISCONNECTED);

	#if defined TW_OnPlayerDisconnect
		return TW_OnPlayerDisconnect(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerUpdate(playerid) {

	pTick[playerid] = gettime();
	GetPlayerPos(playerid, pLastX[playerid], pLastY[playerid], pLastZ[playerid]);

	#if defined TW_OnPlayerUpdate
		return TW_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerText(playerid, text[]) {
	
	pMoveTime[playerid] = gettime();

	#if defined TW_OnPlayerText
		return TW_OnPlayerText(playerid, text[]);
	#else
		return 1;
	#endif
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {

	pMoveTime[playerid] = gettime();

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

			if(gettime() - pTick[i] < 4 && IsPaused[i] == 0) {

				IsPaused[i] = 1;
				pTime[i] = gettime();
				pType[i] = PAUSE_TYPE_TABOUT;

				#if defined PAUSE_LABEL
					pLabel[i] = CreateDynamic3DTextLabel("AFK", 0xFF0000FF, x, y, z+0.9, 15.0, i);
				#endif

				CallLocalFunction("OnPlayerPause", "ii", i, PAUSE_TYPE_TABOUT);
			}
			else if((pLastX[i] == x && pLastY[i] == y && pLastZ[i] == z) && gettime() - pMoveTime[i] < 6 && IsPaused[i] == 0) {

				IsPaused[i] = 1;
				pTime[i] = gettime();
				pMoveTime[i] = gettime();
				pType[i] = PAUSE_TYPE_MOVE;

				#if defined PAUSE_LABEL
					pLabel[i] = CreateDynamic3DTextLabel("AFK", 0xFF0000FF, x, y, z+0.9, 15.0, i);
				#endif

				CallLocalFunction("OnPlayerPause", "ii", i, PAUSE_TYPE_MOVE);
			}
			else {

				IsPaused[i] = 0;

				if(IsValidDynamic3DTextLabel(pLabel[playerid]))
					DestroyDynamic3DTextLabel(pLabel[playerid]);

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

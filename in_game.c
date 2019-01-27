#include "in_game.h"

#include <string.h>

#include "zurapce/zurapce.h"

#include "ArraySize.h"
#include "game_mode.h"
#include "stage_select.h"
#include "camera.h"
#include "player.h"
#include "stage.h"
#include "timer.h"
#include "pad_log.h"

static enum PlayMode
{
	PlayMode_Single,
	PlayMode_VersusGhost,
	PlayMode_Replay,
} g_play_mode;

static struct Stage const* g_stage;
static struct Player g_player;
static struct Player g_ghost;
static struct Camera g_camera;
static struct Timer g_timer;

static struct StartCountdown
{
	int start_frame;
	int end_frame;
	char const* text;
} const s_start_countdown[] = {
	{ 15, 30, "3" },
	{ 45, 60, "2" },
	{ 75, 90, "1" },
	{ 105, 135, "S T A R T" },
};
static int g_start_countdown_frame_count;
static int g_game_frame_count;

void InGame_Init(struct Stage const* stage, int stage_number)
{
	g_stage = stage;
	Player_Construct(&g_player);
	Camera_Construct(&g_camera);
	PadLog_Construct(&g_logging_pad_log, stage, stage_number);
	Timer_Construct(&g_timer);
	g_start_countdown_frame_count = 0;
	g_game_frame_count = 0;
	g_game_mode = GameMode_InGame;
	g_play_mode = PlayMode_Single;
}

void InGame_InitVersusGhost(struct Stage const* stage, int stage_number)
{
	InGame_Init(stage, stage_number);
	Player_Construct(&g_ghost);
	g_play_mode = PlayMode_VersusGhost;
}

void InGame_InitReplay(struct Stage const* stage, int stage_number)
{
	InGame_Init(stage, stage_number);
	g_play_mode = PlayMode_Replay;
}

unsigned long GetPad(enum PlayMode play_mode)
{
	switch(play_mode)
	{
	case PlayMode_Single:
	case PlayMode_VersusGhost:
		return pcePadGet();
	case PlayMode_Replay:
		return PadLog_Get(&g_replay_pad_log, g_game_frame_count);
	default:
		break;
	}
	return 0;
}

void InGame_Update(void)
{
	unsigned long const pad = GetPad(g_play_mode);
	
	struct StartCountdown const* const countdown_last = &s_start_countdown[ARRAY_SIZE(s_start_countdown) - 1];
	if(g_start_countdown_frame_count < countdown_last->end_frame)
	{
		g_start_countdown_frame_count++;
	}
	if(g_start_countdown_frame_count < countdown_last->start_frame)
	{
		return;
	}
	
	Player_Update(&g_player, pad, g_stage);
	if(g_play_mode == PlayMode_VersusGhost)
	{
		Player_Update(&g_ghost, PadLog_Get(&g_replay_pad_log, g_game_frame_count), g_stage);
	}
	Camera_Update(&g_camera, pad, &g_player, g_stage);
	if(g_player.state == PlayerState_Goal)
	{
		if(pcePadGet() & TRG_A)
		{
			PadLog_Copy(&g_replay_pad_log, &g_logging_pad_log);
			StageSelect_Init();
		}
	}
	else
	{
		Timer_Update(&g_timer);
	}
	
	if(g_play_mode == PlayMode_Replay)
	{
		if(pcePadGet() & TRG_A)
		{
			StageSelect_Init();
		}
	}
	else
	{
		PadLog_Log(&g_logging_pad_log, pad, g_game_frame_count);
	}
	
	if(g_player.state != PlayerState_Goal || g_ghost.state != PlayerState_Goal)
	{
		g_game_frame_count++;
	}
}

static void InGame_Player_Draw(enum PlayMode play_mode)
{
	int const draw_in_blink = g_game_frame_count % 2;
	
	switch(play_mode)
	{
	case PlayMode_Single:
		Player_Draw(&g_player, &g_camera);
		break;
	case PlayMode_VersusGhost:
		Player_Draw(&g_player, &g_camera);
		if(draw_in_blink)
		{
			Player_Draw(&g_ghost, &g_camera);
		}
		break;
	case PlayMode_Replay:
		if(draw_in_blink)
		{
			Player_Draw(&g_player, &g_camera);
		}
		break;
	default:
		break;
	}
}

void InGame_Draw(void)
{
	int i;
	
	Stage_Draw(g_stage, &g_camera);
	InGame_Player_Draw(g_play_mode);
	
	pceLCDPaint(0, 0, 0, DISP_X, 8);
	Timer_Draw(&g_timer);
	
	for(i = 0; i < ARRAY_SIZE(s_start_countdown); i++)
	{
		struct StartCountdown const* const countdown = &s_start_countdown[i];
		if(countdown->start_frame <= g_start_countdown_frame_count && g_start_countdown_frame_count < countdown->end_frame)
		{
			FontFuchi_SetTxColor(0);
			FontFuchi_SetBdColor(3);
			FontFuchi_SetType(1);
			FontFuchi_SetPos((DISP_X - 8 * strlen(countdown->text)) / 2, (DISP_Y - 16) / 2);
			FontFuchi_PutStr(countdown->text);
			break;
		}
	}
}

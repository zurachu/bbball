#include "in_game.h"

#include <string.h>

#include "zurapce/zurapce.h"

#include "ArraySize.h"
#include "game_mode.h"
#include "camera.h"
#include "player.h"
#include "stage.h"
#include "timer.h"

static struct Stage const* g_stage;
static struct Player g_player;
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
static int const s_countdown_frames = 50;

void InGame_Init(struct Stage const* stage)
{
	g_stage = stage;
	Player_Construct(&g_player);
	Camera_Construct(&g_camera);
	Timer_Construct(&g_timer);
	g_start_countdown_frame_count = 0;
	g_game_mode = GameMode_InGame;
}

void InGame_Update(void)
{
	unsigned long const pad = pcePadGet();
	
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
	Camera_Update(&g_camera, pad, &g_player, g_stage);
	Timer_Update(&g_timer);
}

void InGame_Draw(void)
{
	int i;
	
	Stage_Draw(g_stage, &g_camera);
	Player_Draw(&g_player, &g_camera);
	
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

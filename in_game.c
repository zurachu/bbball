#include "in_game.h"

#include "zurapce/zurapce.h"

#include "camera.h"
#include "player.h"
#include "stage.h"
#include "timer.h"

static struct Stage const* g_stage;
static struct Player g_player;
static struct Camera g_camera;
static struct Timer g_timer;

void InGame_Init(struct Stage const* stage)
{
	g_stage = stage;
	Player_Construct(&g_player);
	Camera_Construct(&g_camera);
	Timer_Construct(&g_timer);
}

void InGame_Update(void)
{
	Player_Update(&g_player, g_stage);
	Camera_Update(&g_camera, &g_player, g_stage);
	Timer_Update(&g_timer);
}

void InGame_Draw(void)
{
	Stage_Draw(g_stage, &g_camera);
	Player_Draw(&g_player, &g_camera);
	
	pceLCDPaint(0, 0, 0, DISP_X, 8);
	Timer_Draw(&g_timer);
}

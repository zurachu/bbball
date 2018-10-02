#include "zurapce/zurapce.h"

#include "title.h"
#include "in_game.h"
#include "player.h"
#include "stage.h"

static PrecisionTimer g_precision_timer;
static unsigned long g_period_us, g_proc_us;

static enum GameMode
{
	GameMode_Uninitialized,
	GameMode_Title,
	GameMode_InGame,
} g_game_mode = GameMode_Uninitialized;

/// 初期化.
void pceAppInit(void)
{
	FramObject_Init();
	pceAppSetProcPeriod(33);
	Configure_Init();
	FontProxy_Hook_Set();
	FontExtend_Hook_GetAdrs();
	
	if(Lcd_Init())
	{
		Player_Init();
		Stage_Init();
		Title_Init();
		Title_Draw();
		g_game_mode = GameMode_Title;
		
		PrecisionTimer_Construct(&g_precision_timer);
	}
}

/// メイン.
void pceAppProc(int cnt)
{
	PrecisionTimer proc_timer;
	PrecisionTimer_Construct(&proc_timer);
	
	if(g_game_mode == GameMode_Uninitialized || pcePadGet() & TRG_D)
	{
		pceAppReqExit(0);
	}
	
	switch(g_game_mode)
	{
	case GameMode_Uninitialized:
		break;
	case GameMode_Title:
		Title_Update();
		break;
	case GameMode_InGame:
		InGame_Update();
		break;
	}

	switch(g_game_mode)
	{
	case GameMode_Uninitialized:
		break;
	case GameMode_Title:
		Title_Draw();
		break;
	case GameMode_InGame:
		InGame_Draw();
		break;
	}
	
	Lcd_Update();
	Lcd_Trans();

	g_period_us = PrecisionTimer_Count(&g_precision_timer);
	g_proc_us = PrecisionTimer_Count(&proc_timer);
}

/// 終了.
void pceAppExit(void)
{
	Ldirect_Exit();
	FontExtend_Unhook_GetAdrs();
	FontProxy_Unhook_Set();
	Configure_Exit();
}

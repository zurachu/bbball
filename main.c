#include "zurapce/zurapce.h"

#include "in_game.h"
#include "player.h"
#include "stage.h"

static BOOL s_initialize_succeed = FALSE;

PrecisionTimer g_precision_timer;
unsigned long g_period_us, g_proc_us;

/// èâä˙âª.
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
		InGame_Init(&g_stage1);
		
		PrecisionTimer_Construct(&g_precision_timer);
		
		s_initialize_succeed = TRUE;
	}
}

/// ÉÅÉCÉì.
void pceAppProc(int cnt)
{
	PrecisionTimer proc_timer;
	PrecisionTimer_Construct(&proc_timer);
	
	if(!s_initialize_succeed || pcePadGet() & TRG_D)
	{
		pceAppReqExit(0);
	}
	
	InGame_Update();
	InGame_Draw();
	
	Lcd_Update();
	Lcd_Trans();

	g_period_us = PrecisionTimer_Count(&g_precision_timer);
	g_proc_us = PrecisionTimer_Count(&proc_timer);
}

/// èIóπ.
void pceAppExit(void)
{
	Ldirect_Exit();
	FontExtend_Unhook_GetAdrs();
	FontProxy_Unhook_Set();
	Configure_Exit();
}

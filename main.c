#include "zurapce/zurapce.h"

#include "player.h"
#include "stage.h"

static BOOL s_initialize_succeed = FALSE;

PrecisionTimer g_timer;
unsigned long g_period_us, g_proc_us;

static struct Player g_player;
int g_scroll_x_offset;

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
		Player_Construct(&g_player);
		
		Stage_Draw(&g_stage1, 0);
		
		PrecisionTimer_Construct( &g_timer );
		
		s_initialize_succeed = TRUE;
	}
}

/// ÉÅÉCÉì.
void pceAppProc(int cnt)
{
	PrecisionTimer timer;
	PrecisionTimer_Construct(&timer);

	if(!s_initialize_succeed || pcePadGet() & TRG_D)
	{
		pceAppReqExit(0);
	}
	
	Player_Update(&g_player, &g_stage1);
	
	if(pcePadGet() & PAD_RI)
	{
		g_scroll_x_offset++;
	}
	if(pcePadGet() & PAD_LF)
	{
		g_scroll_x_offset--;
	}
	g_scroll_x_offset = Stage_AdjustedScrollOffset(&g_stage1, g_scroll_x_offset);

	Stage_Draw(&g_stage1, g_scroll_x_offset);
	Player_Draw(&g_player, g_scroll_x_offset);
	
	pceLCDPaint(0, 0, 0, DISP_X, 8);
	FontFuchi_SetType(2);
	FontFuchi_SetPos(1, 1);
	FontFuchi_Printf("%6lu/%6luus FREE:%8d", g_proc_us, g_period_us, pceHeapGetMaxFreeSize());
	
	Lcd_Update();
	Lcd_Trans();

	g_period_us = PrecisionTimer_Count(&g_timer);
	g_proc_us = PrecisionTimer_Count(&timer);
}

/// èIóπ.
void pceAppExit(void)
{
	Ldirect_Exit();
	FontExtend_Unhook_GetAdrs();
	FontProxy_Unhook_Set();
	Configure_Exit();
}

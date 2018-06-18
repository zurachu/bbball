#include "zurapce/zurapce.h"

#include "player.h"

static BOOL s_initialize_succeed = FALSE;

PrecisionTimer g_timer;
unsigned long g_period_us, g_proc_us;

#define BLOCK_SIZE (8)
static unsigned short s_stage[] = {
	0x001, 0x001, 0x001, 0x003, 0x007, 0x00F, 0x01F, 0x01F,
	0x001, 0x001, 0x001, 0x021, 0x021, 0x021, 0x021, 0x021,
};

static struct Player g_player;

static float const g_gravity_acc = 9.8;

static void Stage_Draw(void);

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
		
		Stage_Draw();
		
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
	
	Player_Update(&g_player);
	
	Stage_Draw();
	Player_Draw(&g_player);
	
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

void Stage_Draw(void)
{
	int i;
	
	pceLCDPaint(0, 0, 0, DISP_X, DISP_Y);
	
	for(i = 0; i < DISP_X / BLOCK_SIZE; i++)
	{
		unsigned short vertical = s_stage[i];
		int const x = i * BLOCK_SIZE;
		int y = DISP_Y - BLOCK_SIZE;
		while(0 <= y)
		{
			if(vertical & 1)
			{
				pceLCDPaint(3, x, y, BLOCK_SIZE, BLOCK_SIZE);
			}
			vertical >>= 1;
			y -= BLOCK_SIZE;
		}
	}
}

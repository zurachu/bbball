#include "title.h"

#include "zurapce/zurapce.h"

static int g_blink_animation_frame_count;
static int const s_blink_animation_frames = 16;

void PushAButton_Init(void)
{
	g_blink_animation_frame_count = 0;
}

void PushAButton_Update(void)
{
	if(++g_blink_animation_frame_count >= s_blink_animation_frames)
	{
		g_blink_animation_frame_count = 0;
	}
}

void PushAButton_Draw(void)
{
	if(g_blink_animation_frame_count < s_blink_animation_frames / 2)
	{
		FontFuchi_SetTxColor(0);
		FontFuchi_SetBdColor(3);
		FontFuchi_SetType(2);
		FontFuchi_SetPos(38, 70);
		FontFuchi_PutStr("PUSH A BUTTON");
	}
}

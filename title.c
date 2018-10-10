#include "title.h"

#include "zurapce/zurapce.h"

#include "ArraySize.h"
#include "game_mode.h"
#include "stage_select.h"
#include "stage.h"
#include "player.h"
#include "camera.h"

#define STAGE_ONE_LOOP 0x001, 0x001, 0x001, 0x003, 0x007, 0x00F, 0x007, 0x003, 0x001, 0x001, 0x001, 0x003, 0x007, 0x00F, 0x01F, 0x00F, 0x007, 0x003,
static unsigned short s_stage_data[] = {
	STAGE_ONE_LOOP
	STAGE_ONE_LOOP
	STAGE_ONE_LOOP
};
#define STAGE_LENGTH (ARRAY_SIZE(s_stage_data))
static struct Stage const s_stage = {
	s_stage_data,
	STAGE_LENGTH,
	-1, -1 // ‰æ–ÊŠO
};
static int const s_stage_one_loop_width = STAGE_LENGTH * BLOCK_SIZE / 3;

static struct Player g_player;
static struct Camera g_camera;

static struct TextAnimation
{
	int type;
	int start_x;
	int goal_x;
	int y;
	char const* text;
} const s_text_animations[] = {
	{ 0, 46, 41, 26, "d" },
	{ 0, 46, 36, 26, "n" },
	{ 0, 46, 31, 26, "u" },
	{ 0, 46, 26, 26, "o" },
	{ 0, 50, 71, 26, "d" },
	{ 0, 50, 66, 26, "n" },
	{ 0, 50, 61, 26, "u" },
	{ 0, 50, 56, 26, "o" },
	{ 1, 40, 18, 20, "B" },
	{ 1, 48, 48, 20, "B" },
	{ 1, 56, 78, 20, "BALL" },
};
static int g_text_animation_frame_count;
static int const s_text_animation_frames = 10;
static int g_button_blink_animation_frame_count;
static int const s_button_blink_animation_frames = 16;

static int TextAnimationIsEnded(void)
{
	return g_text_animation_frame_count >= s_text_animation_frames;
}

void Title_Init(void)
{
	Player_Construct(&g_player);
	Camera_Construct(&g_camera);
	
	g_text_animation_frame_count = 0;
	g_button_blink_animation_frame_count = 0;
	g_game_mode = GameMode_Title;
}

void Title_Update(void)
{
	static unsigned long const s_pad = PAD_RI;
	Player_Update(&g_player, s_pad, &s_stage);
	if(g_player.x >= s_stage_one_loop_width * 2)
	{
		g_player.x -= s_stage_one_loop_width;
		g_camera.x -= s_stage_one_loop_width;
	}
	Camera_Update(&g_camera, s_pad, &g_player, &s_stage);

	if(TextAnimationIsEnded())
	{
		if(++g_button_blink_animation_frame_count >= s_button_blink_animation_frames)
		{
			g_button_blink_animation_frame_count = 0;
		}
	}
	else
	{
		g_text_animation_frame_count++;
	}

	if(pcePadGet() & TRG_A)
	{
		StageSelect_Init();
	}
}

void Title_Draw(void)
{
	int i;
	
	Stage_Draw(&s_stage, &g_camera);
	Player_Draw(&g_player, &g_camera);
	
	FontFuchi_SetTxColor(0);
	FontFuchi_SetBdColor(3);
	for(i = 0; i < sizeof(s_text_animations) / sizeof(s_text_animations[0]); i++)
	{
		struct TextAnimation const* const animation = &s_text_animations[i];
		int const x = (animation->start_x * (s_text_animation_frames - g_text_animation_frame_count) + animation->goal_x * g_text_animation_frame_count) / s_text_animation_frames;
		FontFuchi_SetType(animation->type);
		FontFuchi_SetPos(x, animation->y);
		FontFuchi_PutStr(animation->text);
	}

	if(TextAnimationIsEnded() && g_button_blink_animation_frame_count < s_button_blink_animation_frames / 2)
	{
		FontFuchi_SetType(2);
		FontFuchi_SetPos(38, 70);
		FontFuchi_PutStr("PUSH A BUTTON");
	}
}

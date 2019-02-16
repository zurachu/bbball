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
#include "selectable_dialog.h"
#include "push_a_button.h"

static enum InGamePlayMode g_play_mode;
static struct Stage const* g_stage;
static int g_stage_number;
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
static char const s_time_over_message[] = "TIME OVER";
static int g_start_countdown_frame_count;
static int g_game_frame_count;
static struct SelectableDialog g_dialog;
static struct SelectableDialog g_error_dialog;

void InGame_Init(enum InGamePlayMode play_mode, struct Stage const* stage, int stage_number)
{
	g_stage = stage;
	g_stage_number = stage_number;
	Player_Construct(&g_player);
	if(play_mode == InGamePlayMode_VersusGhost)
	{
		Player_Construct(&g_ghost);
	}
	Camera_Construct(&g_camera);
	PadLog_Construct(&g_logging_pad_log, stage, stage_number);
	Timer_Construct(&g_timer);
	SelectableDialog_Disable(&g_dialog);
	SelectableDialog_Disable(&g_error_dialog);
	PushAButton_Init();
	g_start_countdown_frame_count = 0;
	g_game_frame_count = 0;
	g_game_mode = GameMode_InGame;
	g_play_mode = play_mode;
}

unsigned long GetPad(enum InGamePlayMode play_mode)
{
	switch(play_mode)
	{
	case InGamePlayMode_Single:
	case InGamePlayMode_VersusGhost:
		return pcePadGet();
	case InGamePlayMode_Replay:
		return PadLog_Get(&g_replay_pad_log, g_game_frame_count);
	default:
		break;
	}
	return 0;
}

static void ErrorDialog(void)
{
	SelectableDialog_Update(&g_error_dialog);
	if(SelectableDialog_IsSelected(&g_error_dialog))
	{
		StageSelect_Init();
	}
}

static void SelectSaveReplayDialog(void)
{
	SelectableDialog_Update(&g_dialog);
	if(SelectableDialog_IsSelected(&g_dialog))
	{
		if(SelectableDialog_SelectedIndex(&g_dialog) == 0 && PadLog_WriteFile(&g_replay_pad_log) != 0)
		{
			static char const* const s_choices[] = {"OK"};
			SelectableDialog_Start(&g_error_dialog, "SAVE FAILED", s_choices, ARRAY_SIZE(s_choices));
		}
		else
		{
			StageSelect_Init();
		}
	}
}

void InGame_Update(void)
{
	unsigned long const pad = GetPad(g_play_mode);
	struct StartCountdown const* const countdown_last = &s_start_countdown[ARRAY_SIZE(s_start_countdown) - 1];

	if(pcePadGet() & TRG_D)
	{
		StageSelect_Init();
	}

	if(g_start_countdown_frame_count < countdown_last->end_frame)
	{
		g_start_countdown_frame_count++;
	}
	if(g_start_countdown_frame_count < countdown_last->start_frame)
	{
		return;
	}

	if(SelectableDialog_IsEnabled(&g_error_dialog))
	{
		ErrorDialog();
		return;
	}

	if(SelectableDialog_IsEnabled(&g_dialog))
	{
		SelectSaveReplayDialog();
		return;
	}

	if(Timer_IsOverMax(&g_timer))
	{
		if(pcePadGet() & TRG_A)
		{
			StageSelect_Init();
		}
	}
	else
	{
		Player_Update(&g_player, pad, g_stage);
		if(g_play_mode == InGamePlayMode_VersusGhost)
		{
			Player_Update(&g_ghost, PadLog_Get(&g_replay_pad_log, g_game_frame_count), g_stage);
		}
		Camera_Update(&g_camera, pad, &g_player, g_stage);
	}

	if(g_player.state == PlayerState_Goal)
	{
		if(g_play_mode != InGamePlayMode_Replay && (pcePadGet() & TRG_A))
		{
			if(!PadLog_IsCompatible(&g_replay_pad_log, g_stage, g_stage_number) || g_logging_pad_log.total_frame <= g_replay_pad_log.total_frame)
			{
				static char const* const s_choices[] = {"YES", "NO"};
				SelectableDialog_Start(&g_dialog, "SAVE REPLAY ?", s_choices, ARRAY_SIZE(s_choices));

				PadLog_Copy(&g_replay_pad_log, &g_logging_pad_log);
			}
			else
			{
				StageSelect_Init();
			}
		}
		PushAButton_Update();
	}
	else
	{
		Timer_Update(&g_timer);
	}

	if(g_play_mode == InGamePlayMode_Replay)
	{
		if(pcePadGet() & TRG_A)
		{
			StageSelect_Init();
		}
	}
	else if(g_player.state != PlayerState_Goal)
	{
		PadLog_Log(&g_logging_pad_log, pad, g_game_frame_count);
	}

	if(!Timer_IsOverMax(&g_timer))
	{
		g_game_frame_count++;
	}
}

static void InGame_Player_Draw(enum InGamePlayMode play_mode)
{
	int const draw_in_blink = g_game_frame_count % 2;

	switch(play_mode)
	{
	case InGamePlayMode_Single:
		Player_Draw(&g_player, &g_camera);
		break;
	case InGamePlayMode_VersusGhost:
		Player_Draw(&g_player, &g_camera);
		if(draw_in_blink)
		{
			Player_Draw(&g_ghost, &g_camera);
		}
		break;
	case InGamePlayMode_Replay:
		if(draw_in_blink)
		{
			Player_Draw(&g_player, &g_camera);
		}
		break;
	default:
		break;
	}
}

static void DrawCenteringMessage(char const* message)
{
	FontFuchi_SetTxColor(0);
	FontFuchi_SetBdColor(3);
	FontFuchi_SetType(1);
	FontFuchi_SetPos((DISP_X - 8 * strlen(message)) / 2, (DISP_Y - 16) / 2);
	FontFuchi_PutStr(message);
}

static char const* GoalMessage(void)
{
	if(g_play_mode == InGamePlayMode_VersusGhost)
	{
		if(g_logging_pad_log.total_frame < g_replay_pad_log.total_frame)
		{
			return "W I N";
		}
		else if(g_logging_pad_log.total_frame > g_replay_pad_log.total_frame)
		{
			return "L O S E";
		}
		else
		{	// PadLog_Copy() して同一になった時点でこの表示に切り替わるが、 SelectableDialog に隠れるので気にしない
			return "D R A W";
		}
	}
	return "G O A L";
}

static void InGame_PushAButton_Draw(void)
{
	if(!SelectableDialog_IsEnabled(&g_dialog) && !SelectableDialog_IsEnabled(&g_error_dialog))
	{
		PushAButton_Draw();
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
			DrawCenteringMessage(countdown->text);
			break;
		}
	}

	if(Timer_IsOverMax(&g_timer))
	{
		DrawCenteringMessage(s_time_over_message);
		InGame_PushAButton_Draw();
	}
	else if(g_player.state == PlayerState_Goal)
	{
		DrawCenteringMessage(GoalMessage());
		InGame_PushAButton_Draw();
	}

	SelectableDialog_Draw(&g_dialog);
	SelectableDialog_Draw(&g_error_dialog);
}

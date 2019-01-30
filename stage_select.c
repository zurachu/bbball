#include "stage_select.h"

#include <string.h>

#include "zurapce/zurapce.h"

#include "ArraySize.h"
#include "game_mode.h"
#include "in_game.h"
#include "stage.h"
#include "selectable_dialog.h"
#include "pad_log.h"

static struct Stage const* const s_stages[] = {
	NULL, // dummy
	&g_stage1,
	&g_stage1,
	&g_stage1,
};
static int const s_stage_interval = 64;

static int g_has_replay_pad_log_file[ARRAY_SIZE(s_stages)];
static int g_selected_stage_index = 1;
static int g_rest_move_x = 0;
static struct SelectableDialog g_dialog;

static void CheckReplayPadLogFile()
{
	FILEINFO pfi;
	int i;
	for(i = 0; i < ARRAY_SIZE(g_has_replay_pad_log_file); i++)
	{
		g_has_replay_pad_log_file[i] = 0;
	}
	
	pceFileFindOpen(&pfi);
	while(pceFileFindNext(&pfi))
	{
		int const stage_number = PadLog_StageNumberFromFileName(pfi.filename);
		if(0 < stage_number && stage_number < ARRAY_SIZE(s_stages)
			&& PadLog_ReadFile(NULL, s_stages[stage_number], stage_number))
		{
			g_has_replay_pad_log_file[stage_number] = 1;
		}
	}
	pceFileFindClose(&pfi);
}

void StageSelect_Init(void)
{
	CheckReplayPadLogFile();
	SelectableDialog_Disable(&g_dialog);
	g_game_mode = GameMode_StageSelect;
}

static void StartInGame(enum InGamePlayMode play_mode)
{
	struct Stage const* const stage = s_stages[g_selected_stage_index];
	
	if(!PadLog_IsCompatible(&g_replay_pad_log, stage, g_selected_stage_index) && g_has_replay_pad_log_file[g_selected_stage_index])
	{
		PadLog_ReadFile(&g_replay_pad_log, stage, g_selected_stage_index);
	}
	InGame_Init(play_mode, stage, g_selected_stage_index);
}

static void SelectPlayModeDialog(void)
{
	SelectableDialog_Update(&g_dialog);
	if(SelectableDialog_IsSelected(&g_dialog))
	{
		enum InGamePlayMode const play_mode = SelectableDialog_SelectedIndex(&g_dialog);
		StartInGame(play_mode);
	}
}

void StageSelect_Update(void)
{
	if(SelectableDialog_IsEnabled(&g_dialog))
	{
		SelectPlayModeDialog();
		if(pcePadGet() & TRG_B)
		{
			SelectableDialog_Disable(&g_dialog);
		}
		return;
	}
	
	if(g_rest_move_x == 0)
	{
		if(pcePadGet() & PAD_LF && g_selected_stage_index > 1)
		{
			g_selected_stage_index--;
			g_rest_move_x = -s_stage_interval;
		}
		if(pcePadGet() & PAD_RI && g_selected_stage_index < ARRAY_SIZE(s_stages) - 1)
		{
			g_selected_stage_index++;
			g_rest_move_x = s_stage_interval;
		}
		if(pcePadGet() & TRG_A)
		{
			struct Stage const* const stage = s_stages[g_selected_stage_index];
			if(PadLog_IsCompatible(&g_replay_pad_log, stage, g_selected_stage_index) || g_has_replay_pad_log_file[g_selected_stage_index])
			{
				static char const* const s_choices[] = {"SINGLE", "VS GHOST", "REPLAY"};
				SelectableDialog_Start(&g_dialog, "SELECT MODE", s_choices, ARRAY_SIZE(s_choices));
			}
			else
			{
				StartInGame(InGamePlayMode_Single);
			}
		}
	}
	
	if(g_rest_move_x < 0)
	{
		g_rest_move_x += 4;
		if(g_rest_move_x > 0)
		{
			g_rest_move_x = 0;
		}
	}
	else if(g_rest_move_x > 0)
	{
		g_rest_move_x -= 4;
		if(g_rest_move_x < 0)
		{
			g_rest_move_x = 0;
		}
	}
}

static void DrawOneStage(struct Stage const* stage, int stage_number, int center_x)
{
	static int const s_stage_center_y = 44;
	static int const s_stage_width = 48;
	static int const s_stage_height = 22;
	static char const s_title_format[] = "STAGE %d";
	char title[sizeof(s_title_format) + 10];
	sprintf(title, s_title_format, stage_number);
	FontFuchi_SetTxColor(0);
	FontFuchi_SetBdColor(3);
	FontFuchi_SetType(0);
	FontFuchi_SetPos(center_x - 5 * strlen(title) / 2, 20);
	FontFuchi_PutStr(title);

	pceLCDPaint(3, center_x - (s_stage_width + 2) / 2, s_stage_center_y - (s_stage_height + 2) / 2, s_stage_width + 2, s_stage_height + 2);
	Stage_DrawForStageSelect(stage, center_x, s_stage_center_y);
}

void StageSelect_Draw(void)
{
	int i;
	pceLCDPaint(0, 0, 0, DISP_X, DISP_Y);
	FontFuchi_SetRange(-s_stage_interval, 0, DISP_X + s_stage_interval, DISP_Y);
	for(i = -2; i <= 2; i++)
	{
		int const index = g_selected_stage_index + i;
		if(1 <= index && index < ARRAY_SIZE(s_stages))
		{
			int const x = DISP_X / 2 + g_rest_move_x + s_stage_interval * i;
			DrawOneStage(s_stages[index], index, x);
		}
	}
	SelectableDialog_Draw(&g_dialog);
}

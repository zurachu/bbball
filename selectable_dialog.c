#include "selectable_dialog.h"

#include <string.h>
#include <piece.h>

void SelectableDialog_Disable(struct SelectableDialog* dialog)
{
	dialog->state = SelectableDialogState_Disabled;
}

void SelectableDialog_Start(struct SelectableDialog* dialog, char const* message, char const* const* choices, int num_choices)
{
	dialog->state = SelectableDialogState_Selecting;
	dialog->selecting = 0;
	dialog->message = message;
	dialog->choices = choices;
	dialog->num_choices = num_choices;
}

int SelectableDialog_IsEnabled(struct SelectableDialog const* dialog)
{
	return dialog->state != SelectableDialogState_Disabled;
}

int SelectableDialog_IsSelected(struct SelectableDialog const* dialog)
{
	return dialog->state == SelectableDialogState_Selected;
}

int SelectableDialog_SelectedIndex(struct SelectableDialog const* dialog)
{
	if(SelectableDialog_IsSelected(dialog))
	{
		return dialog->selecting;
	}
	return SELECTABLE_DIALOG_NOT_SELECTED_VALUE;
}

void SelectableDialog_Update(struct SelectableDialog* dialog)
{
	if(dialog->state == SelectableDialogState_Selecting)
	{
		unsigned long const pad = pcePadGet();
		if(pad & TRG_UP)
		{
			dialog->selecting = (dialog->selecting - 1 + dialog->num_choices) % dialog->num_choices;
		}
		if(pad & TRG_DN)
		{
			dialog->selecting = (dialog->selecting + 1) % dialog->num_choices;
		}
		if(pad & TRG_A)
		{
			dialog->state = SelectableDialogState_Selected;
		}
	}
}

static int FontPutStrWithCentering(char const* text, int y)
{
	pceFontSetPos((DISP_X - 5 * strlen(text)) / 2, y);
	return pceFontPutStr(text);
}

void SelectableDialog_Draw(struct SelectableDialog const* dialog)
{
	if(SelectableDialog_IsEnabled(dialog))
	{
		static int const s_inner_width = 10 * 10;
		static int const s_inner_x = (DISP_X - s_inner_width) / 2;
		static int const s_border_left = s_inner_x - 3;
		static int const s_border_right = s_inner_x + s_inner_width + 3 - 1;
		int const num_lines = 2 + dialog->num_choices;
		int y = (DISP_Y - num_lines * 10) / 2;
		int const inner_height = num_lines * 10;
		int const border_top = y - 3;
		int const border_bottom = y + inner_height + 3 - 1;
		int i;

		pceLCDPaint(0, s_inner_x - 4, y - 4, s_inner_width + 8, inner_height + 8);
		pceLCDLine(3, s_border_left, border_top, s_border_right, border_top);
		pceLCDLine(3, s_border_left, border_bottom, s_border_right, border_bottom);
		pceLCDLine(3, s_border_left, border_top, s_border_left, border_bottom);
		pceLCDLine(3, s_border_right, border_top, s_border_right, border_bottom);

		pceFontSetType(0);
		pceFontSetBkColor(FC_SPRITE);
		pceFontSetTxColor(3);
		FontPutStrWithCentering(dialog->message, y);

		y += 2 * 10;
		for(i = 0; i < dialog->num_choices; i++, y += 10)
		{
			if(i == dialog->selecting)
			{
				pceFontSetTxColor(0);
				pceLCDPaint(3, 14, y, 100, 10);
			}
			else
			{
				pceFontSetTxColor(3);
			}
			FontPutStrWithCentering(dialog->choices[i], y);
		}
	}
}

#if !defined(SELECTABLE_DIALOG_H_201901292334)
#define SELECTABLE_DIALOG_H_201901292334

enum SelectableDialogState
{
	SelectableDialogState_Disabled,
	SelectableDialogState_Selecting,
	SelectableDialogState_Selected,
};

#define SELECTABLE_DIALOG_NOT_SELECTED_VALUE (-1)

struct SelectableDialog
{
	enum SelectableDialogState state;
	int selecting;
	char const* message;
	char const* const* choices;
	int num_choices;
};

void SelectableDialog_Disable(struct SelectableDialog* dialog);
void SelectableDialog_Start(struct SelectableDialog* dialog, char const* message, char const* const* choices, int num_choices);
int SelectableDialog_IsEnabled(struct SelectableDialog const* dialog);
int SelectableDialog_IsSelected(struct SelectableDialog const* dialog);
int SelectableDialog_SelectedIndex(struct SelectableDialog const* dialog);
void SelectableDialog_Update(struct SelectableDialog* dialog);
void SelectableDialog_Draw(struct SelectableDialog const* dialog);

#endif // !defined(SELECTABLE_DIALOG_H_201901292334)

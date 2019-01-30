#if !defined(IN_GAME_H_201810012314)
#define IN_GAME_H_201810012314

struct Stage;

enum InGamePlayMode
{
	InGamePlayMode_Single,
	InGamePlayMode_VersusGhost,
	InGamePlayMode_Replay,
};

void InGame_Init(enum InGamePlayMode play_mode, struct Stage const* stage, int stage_number);
void InGame_Update(void);
void InGame_Draw(void);

#endif // !defined(IN_GAME_H_201810012314)

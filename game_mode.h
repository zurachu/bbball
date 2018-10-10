#if !defined(GAME_MODE_H_201810081200)
#define GAME_MODE_H_201810081200

enum GameMode
{
	GameMode_Uninitialized,
	GameMode_Title,
	GameMode_StageSelect,
	GameMode_InGame,
};
extern enum GameMode g_game_mode;

#endif // !defined(GAME_MODE_H_201810081200)

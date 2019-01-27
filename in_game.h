#if !defined(IN_GAME_H_201810012314)
#define IN_GAME_H_201810012314

struct Stage;

void InGame_Init(struct Stage const* stage, int stage_number);
void InGame_InitVersusGhost(struct Stage const* stage, int stage_number);
void InGame_InitReplay(struct Stage const* stage, int stage_number);
void InGame_Update(void);
void InGame_Draw(void);

#endif // !defined(IN_GAME_H_201810012314)

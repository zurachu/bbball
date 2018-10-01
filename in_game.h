#if !defined(IN_GAME_H_201810012314)
#define IN_GAME_H_201810012314

struct Stage;

void InGame_Init(struct Stage const* stage);
void InGame_Update(void);
void InGame_Draw(void);

#endif // !defined(IN_GAME_H_201810012314)

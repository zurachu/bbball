#if !defined(PLAYER_H_201806190732)
#define PLAYER_H_201806190732

struct Stage;

struct Player
{
	int state;
	float x;
	float y;
	float vx;
	float vy;
};

void Player_Init(void);
void Player_Construct(struct Player* player);
void Player_Update(struct Player* player, struct Stage const* stage);
void Player_Draw(struct Player const* player, int x_offset);

#endif // !defined(PLAYER_H_201806190732)

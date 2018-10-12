#if !defined(PLAYER_H_201806190732)
#define PLAYER_H_201806190732

struct Stage;
struct Camera;

enum PlayerState
{
	PlayerState_CannotControl,
	PlayerState_Landed,
	PlayerState_Jumping,
	PlayerState_Goal
};

struct Player
{
	enum PlayerState state;
	float x;
	float y;
	float vx;
	float vy;
	float last_landed_x;
};

void Player_Init(void);
void Player_Construct(struct Player* player);
void Player_Update(struct Player* player, unsigned long pad, struct Stage const* stage);
void Player_Draw(struct Player const* player, struct Camera const* camera);

#endif // !defined(PLAYER_H_201806190732)

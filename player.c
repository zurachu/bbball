#include "player.h"

#include "zurapce/zurapce.h"

#include "camera.h"
#include "stage.h"

extern unsigned char BALL[];
static PIECE_BMP g_ball;

static float const g_mass = 2;
static float const g_gravity_acc_per_frame = 9.8 / 30;

void Player_Init(void)
{
	PieceBmp_Construct(&g_ball, BALL);
}

void Player_Construct(struct Player* player)
{
	player->state = PlayerState_CannotControl;
	player->x = g_ball.header.w / 2;
	player->y = DISP_Y;
	player->vx = 0;
	player->vy = 0;
}

void Player_Update(struct Player* player, struct Stage const* stage)
{
	float const ball_w = g_ball.header.w;

	switch(player->state)
	{
	case PlayerState_CannotControl:
		break;

	case PlayerState_Landed:
		player->vy = g_mass * 4; // TODO: ’n–Ê‚É‚æ‚Á‚Ä’µ‚Ë•û‚ð•Ï‚¦‚é
		player->state = PlayerState_Jumping;
		break;

	case PlayerState_Jumping:
		if(pcePadGet() & PAD_RI)
		{
			player->vx++;
		}
		if(pcePadGet() & PAD_LF)
		{
			player->vx--;
		}
		player->x += player->vx;
		player->vx = 0;
		break;
	}
	player->vy -= g_mass * g_gravity_acc_per_frame;
	player->x += player->vx;
	player->y += player->vy;
	if(Stage_Block(stage, player->x - ball_w / 2, player->y) == 1
	 || Stage_Block(stage, player->x + ball_w / 2, player->y) == 1)
	{
		do
		{
			player->y++;
		}
		while((int)player->y % BLOCK_SIZE);
		player->state = PlayerState_Landed;
	}
}

void Player_Draw(struct Player const* player, struct Camera const* camera)
{
	int const ball_w = g_ball.header.w;
	int const ball_h = g_ball.header.h;
	PieceBmp_Draw(&g_ball, player->x - ball_w / 2 - camera->x, DISP_Y - player->y - ball_h, 0, 0, ball_w, ball_h, DRW_NOMAL);
}

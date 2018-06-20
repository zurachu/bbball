#include "player.h"

#include "zurapce/zurapce.h"

extern unsigned char BALL[];
static PIECE_BMP g_ball;

static float const g_gravity_acc_per_frame = 9.8 / 30;

void Player_Init(void)
{
	PieceBmp_Construct(&g_ball, BALL);
}

void Player_Construct(struct Player* player)
{
}

void Player_Update(struct Player* player)
{
	if(pcePadGet() & PAD_RI)
	{
		player->vx++;
	}
	if(pcePadGet() & PAD_LF)
	{
		player->vx--;
	}
	player->x += player->vx;
	
	switch(player->state)
	{
	case 0:
		player->vy += g_gravity_acc_per_frame;
		break;
	case 1:
		player->vy = -4;
		player->state = 0;
		break;
	}
	player->y += player->vy;
	if(player->y >= 72)
	{
		player->y = 72;
		player->state = 1;
	}
	player->vx = 0;
}

void Player_Draw(struct Player const* player, int x_offset)
{
	PieceBmp_Draw(&g_ball, player->x - x_offset, player->y, 0, 0, g_ball.header.w, g_ball.header.h, DRW_NOMAL);
}

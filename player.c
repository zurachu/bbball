#include "player.h"

#include "zurapce/zurapce.h"

#include "stage.h"

extern unsigned char BALL[];
static PIECE_BMP g_ball;

static float const g_gravity_acc_per_frame = 9.8 / 30;

void Player_Init(void)
{
	PieceBmp_Construct(&g_ball, BALL);
}

void Player_Construct(struct Player* player)
{
	player->state = 0;
	player->x = g_ball.header.w / 2;
	player->y = DISP_Y;
	player->vx = 0;
	player->vy = 0;
}

void Player_Update(struct Player* player, struct Stage const* stage)
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
		player->vy -= g_gravity_acc_per_frame;
		break;
	case 1:
		player->vy = 4;
		player->state = 0;
		break;
	}
	
	player->y += player->vy;
	if(Stage_Block(stage, player->x, player->y) == 1)
	{
		do
		{
			player->y++;
		}
		while((int)player->y % BLOCK_SIZE);
		player->state = 1;
	}
	player->vx = 0;
}

void Player_Draw(struct Player const* player, int x_offset)
{
	int const ball_w = g_ball.header.w;
	int const ball_h = g_ball.header.h;
	PieceBmp_Draw(&g_ball, player->x - ball_w / 2 - x_offset, DISP_Y - player->y - ball_h, 0, 0, ball_w, ball_h, DRW_NOMAL);
}

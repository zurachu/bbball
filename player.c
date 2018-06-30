#include "player.h"

#include "zurapce/zurapce.h"

#include "camera.h"
#include "stage.h"

extern unsigned char BALL[], ARROW[];
static PIECE_BMP g_ball, g_arrow;

static float const g_mass = 2;
static float const g_gravity_acc_per_frame = 9.8 / 30;
static float const g_display_upper_y_limit = 8;

void Player_Init(void)
{
	PieceBmp_Construct(&g_ball, BALL);
	PieceBmp_Construct(&g_arrow, ARROW);
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
		player->vy = g_mass * 4; // TODO: �n�ʂɂ���Ē��˕���ς���
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
	int const ball_h = g_ball.header.h;
	int const display_y = DISP_Y - player->y - ball_h;
	if(display_y + ball_h >= g_display_upper_y_limit)
	{
		int const ball_w = g_ball.header.w;
		PieceBmp_Draw(&g_ball, player->x - ball_w / 2 - camera->x, display_y, 0, 0, ball_w, ball_h, DRW_NOMAL);
	}
	else
	{
		int const arrow_w = g_arrow.header.w;
		PieceBmp_Draw(&g_arrow, player->x - arrow_w / 2 - camera->x, g_display_upper_y_limit, 0, 0, arrow_w, g_arrow.header.h, DRW_NOMAL);
	}
}

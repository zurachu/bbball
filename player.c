#include "player.h"

#include "zurapce/zurapce.h"

#include "camera.h"
#include "stage.h"

extern unsigned char BALL[], ARROW[];
static PIECE_BMP g_ball, g_arrow;

static float const g_mass = 2;
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

static void Player_Update_Delta(struct Player* player, struct Stage const* stage, int total)
{
	float const ball_w = g_ball.header.w;
	float const ball_h = g_ball.header.h;
	float const dvx = player->vx / total;
	float const dvy = player->vy / total;
	
	player->x += dvx;
	player->y += dvy;

	if(Stage_Block(stage, player->x - ball_w / 2, player->y + ball_h / 2) == 1)
	{
		float const block_right_x = (int)(player->x / BLOCK_SIZE) * BLOCK_SIZE;
		player->x += (block_right_x - (player->x - ball_w / 2)) * 2;
		player->state = PlayerState_CannotControl;
		player->vx = -player->vx;
	}
	else if(Stage_Block(stage, player->x + ball_w / 2, player->y + ball_h / 2) == 1)
	{
		float const block_left_x = (int)(player->x / BLOCK_SIZE + 1) * BLOCK_SIZE;
		player->x += (block_left_x - (player->x + ball_w / 2)) * 2;
		player->state = PlayerState_CannotControl;
		player->vx = -player->vx;
	}

	if(Stage_Block(stage, player->x - ball_w / 2, player->y + ball_h) == 1
	 || Stage_Block(stage, player->x + ball_w / 2, player->y + ball_h) == 1)
	{
		float const block_bottom_y = (int)(player->y / BLOCK_SIZE + 1) * BLOCK_SIZE;
		player->y += (block_bottom_y - (player->y + ball_h)) * 2;
		player->vy = -player->vy;
	}

	if(Stage_Block(stage, player->x - ball_w / 2, player->y) == 1
	 || Stage_Block(stage, player->x + ball_w / 2, player->y) == 1)
	{
		float const block_top_y = (int)(player->y / BLOCK_SIZE + 1) * BLOCK_SIZE;
		player->y = block_top_y;
		player->state = PlayerState_Landed;
		player->vy = 0;
	}
}

static int Max(int a, int b)
{
	return a > b ? a : b;
}

static int RequiredDeltaCount(struct Player* player)
{
	int const vx_count = abs(player->vx);
	int const vy_count = abs(player->vy);
	return Max(1, Max(vx_count, vy_count));
}

void Player_Update(struct Player* player, struct Stage const* stage)
{
	float const gravity_acc_per_frame = 9.8 / (1000.0 / pceAppSetProcPeriod(INVALIDVAL));
	int i, delta_count;

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
			player->vx = 1;
		}
		else if(pcePadGet() & PAD_LF)
		{
			player->vx = -1;
		}
		else
		{
			player->vx = 0;
		}
		break;
	}
	player->vy -= g_mass * gravity_acc_per_frame;
	delta_count = RequiredDeltaCount(player);
	for(i = 0; i < delta_count; i++)
	{
		Player_Update_Delta(player, stage, delta_count);
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

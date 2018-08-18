#include "stage.h"

#include "zurapce/zurapce.h"

#include "camera.h"

extern unsigned char GOAL[];
static PIECE_BMP g_goal;

void Stage_Init(void)
{
	PieceBmp_Construct(&g_goal, GOAL);
}

int Stage_Block(struct Stage const* stage, int x, int y)
{
	int const block_x = x / BLOCK_SIZE;
	int const block_y = y / BLOCK_SIZE;
	if(block_y < 0)
	{
		return 0;
	}
	return (stage->data[block_x] >> block_y) & 1;
}

static void Goal_Draw(struct Stage const* stage, struct Camera const* camera)
{
	int const goal_h = g_goal.header.h;
	int const goal_x = stage->goal_x * BLOCK_SIZE - camera->x;
	int const goal_y = DISP_Y - stage->goal_y * BLOCK_SIZE - goal_h;
	PieceBmp_Draw(&g_goal, goal_x, goal_y, 0, 0, g_goal.header.w, goal_h, DRW_NOMAL);
}

void Stage_Draw(struct Stage const* stage, struct Camera const* camera)
{
	int x_offset = camera->x;
	int const block_offset = x_offset / BLOCK_SIZE;
	int i;
	x_offset -= block_offset * BLOCK_SIZE;
	
	pceLCDPaint(0, 0, 0, DISP_X, DISP_Y);
	
	for(i = 0; i < DISP_X / BLOCK_SIZE + 1; i++)
	{
		unsigned short vertical = stage->data[block_offset + i];
		int const x = i * BLOCK_SIZE;
		int y = DISP_Y - BLOCK_SIZE;
		while(0 <= y)
		{
			if(vertical & 1)
			{
				pceLCDPaint(3, x - x_offset, y, BLOCK_SIZE, BLOCK_SIZE);
			}
			vertical >>= 1;
			y -= BLOCK_SIZE;
		}
	}

	Goal_Draw(stage, camera);
}

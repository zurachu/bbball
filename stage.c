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

int Stage_Goal(struct Stage const* stage, int x, int y)
{
	int const block_x = x / BLOCK_SIZE;
	int const block_y = y / BLOCK_SIZE;
	return block_x == stage->goal_x && block_y == stage->goal_y;
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

	for(i = 0; i <= DISP_X / BLOCK_SIZE; i++)
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

void Stage_DrawForStageSelect(struct Stage const* stage, int center_x, int center_y)
{
	static int const s_block_size = 2;
	static int const s_block_length = 24;
	int const width = s_block_length * s_block_size;
	int const height = DISP_Y * s_block_size / BLOCK_SIZE;
	int const left_x = center_x - width / 2;
	int const top_y = center_y - height / 2;
	int i;

	pceLCDPaint(0, left_x, top_y, width, height);

	for(i = 0; i < s_block_length; i++)
	{
		unsigned short vertical = stage->data[i];
		int const x = left_x + i * s_block_size;
		int y = top_y + height - s_block_size;
		while(top_y <= y)
		{
			if(vertical & 1)
			{
				pceLCDPaint(3, x, y, s_block_size, s_block_size);
			}
			vertical >>= 1;
			y -= s_block_size;
		}
	}
}

unsigned long Stage_CRC(struct Stage const* stage)
{
	unsigned long const len = stage->length * sizeof(stage->data[0]);
	pceCRC32(NULL, 0);
	return pceCRC32(stage->data, len);
}

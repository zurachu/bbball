#include "stage.h"

#include "zurapce/zurapce.h"

int Stage_Block(struct Stage const* stage, int x, int y)
{
	int const block_x = x / BLOCK_SIZE;
	int const block_y = y / BLOCK_SIZE;
	return (stage->data[block_x] >> block_y) & 1;
}

int Stage_AdjustedScrollOffset(struct Stage const* stage, int origin_x_offset)
{
	int const x_offset_limit = stage->length * BLOCK_SIZE - DISP_X;
	if(origin_x_offset < 0 || x_offset_limit < 0)
	{
		return 0;
	}
	if(x_offset_limit < origin_x_offset)
	{
		return x_offset_limit;
	}
	return origin_x_offset;
}

void Stage_Draw(struct Stage const* stage, int x_offset)
{
	int block_offset = x_offset / BLOCK_SIZE;
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
}

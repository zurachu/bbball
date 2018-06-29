#include "stage.h"

#include "zurapce/zurapce.h"

#include "camera.h"

int Stage_Block(struct Stage const* stage, int x, int y)
{
	int const block_x = x / BLOCK_SIZE;
	int const block_y = y / BLOCK_SIZE;
	return (stage->data[block_x] >> block_y) & 1;
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
}

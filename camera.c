#include "camera.h"

#include "zurapce/zurapce.h"

#include "player.h"
#include "stage.h"

void Camera_Construct(struct Camera* camera)
{
	camera->x = 0;
	camera->last_direction = PAD_RI;
}

void Camera_Update(struct Camera* camera, struct Player const* player, struct Stage const* stage)
{
	int const x_limit = stage->length * BLOCK_SIZE - DISP_X;
	int x = camera->x;
	unsigned long last_direction = camera->last_direction;
	int target_x = player->x - DISP_X / 2;
	
	if(pcePadGet() & PAD_RI)
	{
		last_direction = PAD_RI;
	}
	if(pcePadGet() & PAD_LF)
	{
		last_direction = PAD_LF;
	}

	if(last_direction == PAD_RI)
	{
		target_x += DISP_X / 4;
	}
	if(last_direction == PAD_LF)
	{
		target_x -= DISP_X / 4;
	}
	
	if(x < target_x)
	{
		x++;
	}
	if(target_x < x)
	{
		x--;
	}
	
	if(x < 0 || x_limit < 0)
	{
		x = 0;
	}
	if(x_limit < x)
	{
		x = x_limit;
	}
	
	camera->x = x;
	camera->last_direction = last_direction;
}

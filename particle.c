#include "particle.h"

#include "zurapce/zurapce.h"

#include "player.h"
#include "camera.h"

static float const g_mass = 0.25;

void Particle_Construct(struct Particle* particle)
{
	particle->active = 0;
	particle->x = 0;
	particle->y = 0;
	particle->vx = 0;
	particle->vy = 0;
}

static float Random(float from, float to)
{
	if(to < from)
	{
		float tmp = from;
		from = to;
		to = tmp;
	}
	return from + (float)rand() / RAND_MAX * (to - from);
}

void Particle_Start(struct Particle* particle, struct Player const* player)
{
	particle->active = 1;
	particle->x = player->x;
	particle->y = player->y;
	particle->vx = Random(-0.5, 0.5);
	particle->vy = g_mass * Random(2, 4);
	if(player->state == PlayerState_Goal)
	{
		particle->vx *= 2;
		particle->vy *= 2;
	}
}

int Particle_IsActive(struct Particle const* particle)
{
	return particle->active;
}

void Particle_Update(struct Particle* particle)
{
	if(particle->active)
	{
		float const gravity_acc_per_frame = 9.8 / (1000.0 / pceAppSetProcPeriod(INVALIDVAL));
		particle->vy -= g_mass * gravity_acc_per_frame;
		particle->x += particle->vx;
		particle->y += particle->vy;
		if(particle->y < 0)
		{
			particle->active = 0;
		}
	}
}

void Particle_Draw(struct Particle const* particle, struct Camera const* camera)
{
	if(particle->active)
	{
		int const display_x = particle->x - camera->x;
		int const display_y = DISP_Y - particle->y;
		if(0 <= display_x && display_x < DISP_X && 0 <= display_y && display_y < DISP_Y)
		{
			unsigned char* const p = pceLCDSetBuffer(INVALIDPTR) + display_y * DISP_X + display_x;
			*p = (*p == COLOR_BLACK) ? COLOR_WHITE : COLOR_BLACK;
//			pceLCDPoint(3, display_x, display_y);
//			pceLCDPaint(3, display_x, display_y,2,2);
		}
	}
}

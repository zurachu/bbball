#if !defined(PARTICLE_H_201901271546)
#define PARTICLE_H_201901271546

struct Player;
struct Camera;

struct Particle
{
	int active;
	float x;
	float y;
	float vx;
	float vy;
};

void Particle_Construct(struct Particle* particle);
void Particle_Start(struct Particle* particle, struct Player const* player);
int Particle_IsActive(struct Particle const* particle);
void Particle_Update(struct Particle* particle);
void Particle_Draw(struct Particle const* particle, struct Camera const* camera);

#endif // !defined(PARTICLE_H_201901271546)

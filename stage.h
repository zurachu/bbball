#if !defined(STAGE_H_201806192305)
#define STAGE_H_201806192305

#define BLOCK_SIZE (8)

struct Camera;

struct Stage
{
	unsigned short const* data;
	int length;
};

extern struct Stage const g_stage1;

int Stage_Block(struct Stage const* stage, int x, int y);
void Stage_Draw(struct Stage const* stage, struct Camera const* camera);

#endif // !defined(STAGE_H_201806192305)

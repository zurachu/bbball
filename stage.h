#if !defined(STAGE_H_201806192305)
#define STAGE_H_201806192305

#define BLOCK_SIZE (8)

struct Stage
{
	unsigned short const* data;
	int length;
};

extern struct Stage const g_stage1;

int Stage_AdjustedScrollOffset(struct Stage const* stage, int origin_x_offset);
void Stage_Draw(struct Stage const* stage, int x_offset);

#endif // !defined(STAGE_H_201806192305)

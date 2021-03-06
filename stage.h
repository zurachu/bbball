#if !defined(STAGE_H_201806192305)
#define STAGE_H_201806192305

#define BLOCK_SIZE (8)

struct Camera;

struct Stage
{
	unsigned short const* data;
	int length;
	int goal_x;
	int goal_y;
};


void Stage_Init(void);
int Stage_Block(struct Stage const* stage, int x, int y);
int Stage_Goal(struct Stage const* stage, int x, int y);
void Stage_Draw(struct Stage const* stage, struct Camera const* camera);
void Stage_DrawForStageSelect(struct Stage const* stage, int center_x, int center_y);
unsigned long Stage_CRC(struct Stage const* stage);

#endif // !defined(STAGE_H_201806192305)

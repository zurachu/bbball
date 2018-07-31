#include "stage.h"

static unsigned short s_stage_data[] = {
	0x001, 0x001, 0x001, 0x003, 0x007, 0x00F, 0x01F, 0x01F,
	0x001, 0x001, 0x001, 0x000, 0x000, 0x021, 0x021, 0x021,
	0x021, 0x021, 0x021, 0x021, 0x021, 0x021, 0x021, 0x021,
};

struct Stage const g_stage1 = {
	s_stage_data,
	sizeof(s_stage_data) / sizeof(s_stage_data[0])
};

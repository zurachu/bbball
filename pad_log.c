#include "pad_log.h"

#include <string.h>

#include "ArraySize.h"

#include "stage.h"

struct PadLog g_replay_pad_log;
struct PadLog g_logging_pad_log;

void PadLog_Construct(struct PadLog* pad_log, struct Stage const* stage, int stage_number)
{
	pad_log->stage_number = stage_number;
	pad_log->stage_crc = Stage_CRC(stage);
	pad_log->total_frame = 0;
	memset(pad_log->pad, 0, sizeof(pad_log->pad));
}

void PadLog_Log(struct PadLog* pad_log, unsigned long pad, int current_frame)
{
	if(ARRAY_SIZE(pad_log->pad) <= current_frame)
	{
		return;
	}
	
	pad_log->pad[current_frame] = pad & 0xFF;
	if(pad_log->total_frame < current_frame)
	{
		pad_log->total_frame = current_frame;
	}
}

unsigned long PadLog_Get(struct PadLog const* pad_log, int current_frame)
{
	unsigned long const pad = pad_log->pad[current_frame];
	unsigned long previous_pad = 0;
	if(pad_log->total_frame <= current_frame)
	{
		return 0;
	}
	
	if(0 < current_frame)
	{
		previous_pad = pad_log->pad[current_frame - 1];
	}

	return pad | ((~previous_pad & pad) << 8);
}

void PadLog_Copy(struct PadLog* dst_pad_log, struct PadLog const* src_pad_log)
{
	dst_pad_log->stage_number = src_pad_log->stage_number;
	dst_pad_log->stage_crc = src_pad_log->stage_crc;
	dst_pad_log->total_frame = src_pad_log->total_frame;
	memcpy(dst_pad_log->pad, src_pad_log->pad, sizeof(dst_pad_log->pad));
}

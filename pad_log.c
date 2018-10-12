#include "pad_log.h"

struct PadLog g_logging_pad_log;

void PadLog_Construct(struct PadLog* pad_log)
{
	pad_log->current_frame = 0;
	pad_log->total_frame = 0;
}

void PadLog_Update(struct PadLog* pad_log, unsigned long pad)
{
	pad_log->pad[pad_log->current_frame] = pad & 0xFF;
	pad_log->current_frame++;
	pad_log->total_frame++;
}

unsigned long PadLog_Get(struct PadLog* pad_log)
{
	int const current_frame = pad_log->current_frame;
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
	pad_log->current_frame++;

	return pad | ((~previous_pad & pad) << 8);
}

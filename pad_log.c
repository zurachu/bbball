#include "pad_log.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
int errno;
#include <string.h>
#include <piece.h>

#include "ArraySize.h"

#include "stage.h"

struct PadLog g_replay_pad_log;
struct PadLog g_logging_pad_log;

static char const s_filename_format[] = "bbb%05d.sav";

void PadLog_Construct(struct PadLog* pad_log, struct Stage const* stage, int stage_number)
{
	pad_log->version = PAD_LOG_VERSION;
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

int PadLog_IsCompatible(struct PadLog const* pad_log, struct Stage const* stage, int stage_number)
{
	return pad_log->version == PAD_LOG_VERSION
		&& pad_log->stage_number == stage_number
		&& pad_log->stage_crc == Stage_CRC(stage);
}

void PadLog_Copy(struct PadLog* dst_pad_log, struct PadLog const* src_pad_log)
{
	dst_pad_log->version = src_pad_log->version;
	dst_pad_log->stage_number = src_pad_log->stage_number;
	dst_pad_log->stage_crc = src_pad_log->stage_crc;
	dst_pad_log->total_frame = src_pad_log->total_frame;
	memcpy(dst_pad_log->pad, src_pad_log->pad, sizeof(dst_pad_log->pad));
}

int PadLog_StageNumberFromFileName(char const* filename)
{
	char expected[16];
	char const* p = filename;
	int stage_number = 0;
	while(*p != '\0')
	{
		if(isdigit(*p))
		{
			stage_number = atoi(p);
			break;
		}
		p++;
	}
	sprintf(expected, s_filename_format, stage_number);
	
	if(strcmp(filename, expected) == 0)
	{
		return stage_number;
	}
	return 0;
}

int PadLog_ReadFile(struct PadLog* pad_log, struct Stage const* stage, int stage_number)
{
	FILEACC pfa;
	char filename[16];
	sprintf(filename, s_filename_format, stage_number);
	if(pceFileOpen(&pfa, filename, FOMD_RD) == 0)
	{
		int const read_bytes = pceFileReadSct(&pfa, NULL, 0, sizeof(struct PadLog));
		struct PadLog const* const read_pad_log = (struct PadLog const*)(pfa.aptr);
		pceFileClose(&pfa);
		if(read_bytes == sizeof(struct PadLog) && PadLog_IsCompatible(read_pad_log, stage, stage_number))
		{
			if(pad_log != NULL)
			{
				PadLog_Copy(pad_log, read_pad_log);
			}
			return read_bytes;
		}
	}
	return 0;
}

int PadLog_WriteFile(struct PadLog const* pad_log)
{
	FILEACC pfa;
	char filename[16];
	int result;
	sprintf(filename, s_filename_format, pad_log->stage_number);
	result = pceFileCreate(filename, sizeof(struct PadLog));
	if(result != 0)
	{
		return result;
	}
	result = pceFileOpen(&pfa, filename, FOMD_WR);
	if(result != 0)
	{
		return result;
	}
	result = pceFileWriteSct(&pfa, pad_log, 0, sizeof(struct PadLog));
	pceFileClose(&pfa);
	if(result == 0)
	{
		return -1;
	}
	return 0;
}

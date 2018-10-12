#if !defined(PAD_LOG_H_201810130037)
#define PAD_LOG_H_201810130037

struct PadLog
{
	int stage_number;
	int current_frame;
	int total_frame;
	unsigned char pad[4000];
};

extern struct PadLog g_logging_pad_log;

void PadLog_Construct(struct PadLog* pad_log);
void PadLog_Update(struct PadLog* pad_log, unsigned long pad);
unsigned long PadLog_Get(struct PadLog* pad_log);

#endif // !defined(PAD_LOG_H_201810130037)

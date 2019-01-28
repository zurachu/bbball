#if !defined(PAD_LOG_H_201810130037)
#define PAD_LOG_H_201810130037

// 挙動処理の変更などで、過去のログが使えなくなるタイミングでバージョンアップする
#define PAD_LOG_VERSION (1)

struct Stage;

struct PadLog
{
	int version;
	int stage_number;
	unsigned long stage_crc;
	int total_frame;
	unsigned char pad[4000];
};

extern struct PadLog g_replay_pad_log;
extern struct PadLog g_logging_pad_log;

void PadLog_Construct(struct PadLog* pad_log, struct Stage const* stage, int stage_number);
void PadLog_Log(struct PadLog* pad_log, unsigned long pad, int current_frame);
unsigned long PadLog_Get(struct PadLog const* pad_log, int current_frame);
int PadLog_IsCompatible(struct PadLog const* pad_log, struct Stage const* stage, int stage_number);
void PadLog_Copy(struct PadLog* dst_pad_log, struct PadLog const* src_pad_log);

#endif // !defined(PAD_LOG_H_201810130037)

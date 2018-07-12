#include "timer.h"

#include "zurapce/zurapce.h"

#define SEC_PER_MIN (60)

void Timer_Construct(struct Timer* timer)
{
	timer->ms = 0;
}

void Timer_Update(struct Timer* timer)
{
	static unsigned int const s_max_ms = 100 * SEC_PER_MIN * 1000 - 1;
	unsigned int new_ms = timer->ms + pceAppSetProcPeriod(INVALIDVAL);
	if(s_max_ms < new_ms)
	{
		new_ms = s_max_ms;
	}
	timer->ms = new_ms;
}

void Timer_Draw(struct Timer const* timer)
{
	unsigned int const ms = timer->ms;
	FontFuchi_SetType(2);
	FontFuchi_SetPos(1, 1);
	FontFuchi_Printf("%02lu'%02lu\"%03lu", ms / (SEC_PER_MIN * 1000), (ms / 1000) % SEC_PER_MIN, ms % 1000);
}

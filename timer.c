#include "timer.h"

#include "zurapce/zurapce.h"

static unsigned int const s_max_ms = 100 * 1000 - 1;

void Timer_Construct(struct Timer* timer)
{
	timer->ms = 0;
}

int Timer_IsOverMax(struct Timer const* timer)
{
	return s_max_ms <= timer->ms;
}

void Timer_Update(struct Timer* timer)
{
	unsigned int new_ms = timer->ms + pceAppSetProcPeriod(INVALIDVAL);
	if(s_max_ms <= new_ms)
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
	FontFuchi_Printf("TIME: %02lu\"%03lu", ms / 1000, ms % 1000);
}

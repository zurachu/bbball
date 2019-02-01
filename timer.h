#if !defined(TIMER_H_201807122306)
#define TIMER_H_201807122306

struct Timer
{
	unsigned int ms;
};

void Timer_Construct(struct Timer* timer);
int Timer_IsOverMax(struct Timer const* timer);
void Timer_Update(struct Timer* timer);
void Timer_Draw(struct Timer const* timer);

#endif // !defined(TIMER_H_201807122306)

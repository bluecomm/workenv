#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "spu_timer.h"
#include "spu_list.h"



void timer_cb(void *data)
{
	struct gmr_timer_list *timer =
			(struct gmr_timer_list *)data;
	printf("Hello, timer !  %p\n",timer);
	gmr_timer_schedule(timer, 2, 0);
	return;
}


time_t p;
unsigned int g_timer_id;

int main(int argc, char **argv)
{
	struct spu_timer_list timer;//, timer1;

	timer.cb = timer_cb;
	timer.data = &timer;

	gmr_timer_schedule(&timer, 2, 0);

	while(1)
	{

		gmr_timers_check();
		//gmr_timers_prepare();
		gmr_timers_update();
		//sleep(1);
	}
	return 0;
}

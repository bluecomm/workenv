#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_RUN_TIMES 5

typedef struct timer_manage{
	int timer_id;
	int para_a;
	int para_b;
	int counter;
	int counter_ori;
	int (*func)(int a, int b);
	int run_times;
	struct timer_manage *next;
}timer_manage_t;

int keep_running = 1;
int g_timer_id;
timer_manage_t *g_timer_head;

int test_timer_func(int a, int b)
{
	printf("time expires, a = %d, b = %d\n", a, b);
	return 0;
}


int test_timer_add(int para_a, int para_b, int counter,
		int (*func)(int a, int b))
{
	timer_manage_t *ptr;
	timer_manage_t *tmp;
	tmp = malloc(sizeof(timer_manage_t));
	if (tmp == NULL)
		return -1;

	memset(tmp, 0, sizeof(timer_manage_t));

	tmp->para_a = para_a;
	tmp->para_b = para_b;
	tmp->counter = counter;
	tmp->counter_ori = counter;
	tmp->func = func;
	tmp->timer_id = g_timer_id++;

	if(g_timer_head == NULL)
		g_timer_head = tmp;
	else
	{
		ptr = g_timer_head;
		while(ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = tmp;
	}
	return 0;
}


int test_timer_delete(int timer_id)
{
	int counter_timers = 0;
	timer_manage_t *ptr;
	timer_manage_t *ptr_priv = NULL;

	ptr = g_timer_head;
	if(ptr == NULL)
		return -1;

	while(ptr != NULL)
	{
		if(ptr->timer_id == timer_id)
		{
			if (ptr_priv == NULL)
				g_timer_head = g_timer_head->next;
			else
			{
				ptr_priv->next = ptr->next;
			}
			free(ptr);
			break;
		}
		else
		{
			ptr_priv = ptr;
			ptr = ptr->next;
		}
	}

	for(ptr = g_timer_head;ptr;ptr = ptr->next)
		counter_timers ++;
	printf("test_timer_delete(), total timers: %d\n", counter_timers);

	if(counter_timers == 0)
		keep_running = 0;

	return 0;
}

void test_timer_schedule(int a)
{
	timer_manage_t *ptr;
	timer_manage_t *ptr_tmp;

	if(g_timer_head == NULL)
	{
		sleep(1);
		return;
	}
	ptr = g_timer_head;
	while(ptr != NULL)
	{
		ptr->counter--;
		if(ptr->counter == 0 )
		{
			ptr->func(ptr->para_a, ptr->para_b);
			ptr->counter = ptr->counter_ori;
			ptr->run_times++;

			if(ptr->run_times == MAX_RUN_TIMES)
			{
				ptr_tmp = ptr;
				ptr = ptr->next;
				test_timer_delete(ptr_tmp->timer_id);
			}
		}
		else
			ptr = ptr->next;
	}

	return;
}

void test_timer_init(struct itimerval *itimer1,struct
		itimerval *itimer2)
{
	g_timer_id = 0;
	g_timer_head = NULL;

	signal(SIGALRM,test_timer_schedule);
//	signal(SIGVTALARM,test_timer_schedule);

	setitimer(ITIMER_REAL, itimer1, itimer2);
//	setitimer(ITIMER_VIRTUAL, itimer1, itimer2);

	return;
}

void test_timer_destroy(void)
{
	timer_manage_t *ptr;
	timer_manage_t *ptr_tmp = NULL;

	ptr = g_timer_head;
	while(ptr != NULL)
	{
		ptr_tmp = ptr;
		ptr = ptr->next;
		free(ptr_tmp);
	}

	g_timer_head = NULL;

	return;
}


//void stop_running(int a)
//{
//	running = 0;
//}


int main(void)
{
	int counter_timers = 0;
	timer_manage_t *ptr;
	struct itimerval value;
	struct itimerval ovalue;
	value.it_value.tv_sec = 1;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 1;
	value.it_interval.tv_usec = 0;

//	signal(SIGTERM, stop_running);
//	signal(SIGINT, stop_running);

	test_timer_init(&value, &ovalue);
	test_timer_add(1,2,3,test_timer_func);
	test_timer_add(4,5,6,test_timer_func);

	for(ptr = g_timer_head;ptr;ptr = ptr->next)
		counter_timers ++;
	printf("main(), total timers: %d\n", counter_timers);

	while(keep_running)
	{

	}

	test_timer_destroy();

	counter_timers = 0;
	for(ptr = g_timer_head;ptr;ptr = ptr->next)
		counter_timers ++;
	printf("main(), total timers: %d\n", counter_timers);

	return 0;
}

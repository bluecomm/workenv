/*! \defgroup timer Sylincom timers
 *  @{
 */

/*! \file timer.h
 *  \brief Sylincom timer handling routines
 */

#ifndef _VX_TIMER_H_
#define _VX_TIMER_H_

#include <sys/times.h>
#include "spu_common.h"
#include "spu_list.h"
#include "spu_rbtree.h"

#define MAX_SPU_TIME_SEC 0xFFFFFFFF
#define TIME_OUT_LOOP 1
#define TIME_OUT_NORMAL 0

#define PROTECT_TIME_OUT 600

typedef struct my_real_timer_s {
	UINT32 tv_sec; /* Seconds.  */
	UINT32 tv_usec; /* Microseconds.  */
} my_real_timer_t;

typedef struct spu_timer_s {
	struct rb_root root;
	my_real_timer_t nearest;
	my_real_timer_t *nearest_p;
	CHAR timer_name[16];
} spu_timer_t;

/**
 * Timer management:
 *      - Create a struct spu_timer_list
 *      - Fill out timeout and use add_timer or
 *        use schedule_timer to schedule a timer in
 *        x seconds and microseconds from now...
 *      - Use del_timer to remove the timer
 *
 *  Internally:
 *      - We hook into select.c to give a timeval of the
 *        nearest timer. On already passed timers we give
 *        it a 0 to immediately fire after the select
 *      - update_timers will call the callbacks and remove
 *        the timers.
 *
 */
/*! \brief A structure representing a single instance of a timer */
struct spu_timer_list {
	struct rb_node node; /*!< \brief rb-tree node header */
	struct list_creat_head list; /*!< \brief internal list header */
	my_real_timer_t timeout; /*!< \brief expiration time */
	UINT32 active :1; /*!< \brief is it active? */

	VOID (*cb)(VOID*); /*!< \brief call-back called at timeout */
	VOID *data; /*!< \brief user data for callback */
	spu_timer_t * timer_p;
	UINT8 timeout_flag; //TIME_OUT_NORMAL 0 TIME_OUT_LOOP 1
};

/**
 *  init sbs timer
 */

spu_timer_t* get_spu_timer(const CHAR*timer_name);

/**
 * timer management
 */

VOID spu_timer_add(struct spu_timer_list *timer);

VOID spu_timer_schedule(struct spu_timer_list *timer, UINT32 seconds,
		UINT32 microseconds);

VOID spu_timer_del(struct spu_timer_list *timer);

INT32 spu_timer_pending(struct spu_timer_list *timer);

INT32 spu_timer_remaining(const struct spu_timer_list *timer, const my_real_timer_t *now,
		my_real_timer_t *remaining);
/*
 * internal timer list management
 */
my_real_timer_t *spu_timers_nearest(spu_timer_t* timer_p);
VOID spu_timers_prepare(spu_timer_t *timer_p);
INT32 spu_timers_update(spu_timer_t *timer_p);
INT32 spu_timers_check(spu_timer_t *timer_p);

INT32 spu_gettime(my_real_timer_t *tv);

/*! @} */

#endif

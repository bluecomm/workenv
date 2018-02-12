/* These store the amount of time that we wait until next timer expires. */
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include "spu_timer.h"
#include "spu_timer_compat.h"
#include "spu_list.h"

static spu_timer_t spu_timer;
static struct timeval nearest;

INT32 spu_gettime(my_real_timer_t *tv) {
	struct timezone tz;

	if (gettimeofday((struct timeval *) tv, &tz)) {
		log_msg(EROR_LOG, CSL, "gettimeofday failed!\n");
		return ERROR;
	}

	return OK;
}

spu_timer_t* get_spu_timer(const CHAR*timer_name) {
	memcpy(spu_timer.timer_name, timer_name, strlen(timer_name) + 1);
	return &spu_timer;
}

static VOID __add_timer(struct spu_timer_list *timer, spu_timer_t *timer_p) {
	struct rb_node **new = &(timer_p->root.rb_node);
	struct rb_node *parent = NULL;

	while (*new) {
		struct spu_timer_list *this = NULL;

		this = container_of(*new, struct spu_timer_list, node);

		parent = *new;
		if (timercmp(&timer->timeout, &this->timeout, <))
			new = &((*new)->rb_left);
		else
			new = &((*new)->rb_right);
	}

	rb_link_node(&timer->node, parent, new);
	rb_insert_color(&timer->node, &timer_p->root);
}

/*! \brief add a new timer to the timer management
 *  \param[in] timer the timer that should be added
 */
VOID spu_timer_add(struct spu_timer_list *timer) {
	spu_timer_del(timer);
	timer->active = 1;
	INIT_LIST_HEAD(&timer->list);
	__add_timer(timer, timer->timer_p);
}

/*! \brief schedule a timer at a given future relative time
 *  \param[in] timer the to-be-added timer
 *  \param[in] seconds number of seconds from now
 *  \param[in] microseconds number of microseconds from now
 *
 * This function can be used to (re-)schedule a given timer at a
 * specified number of seconds+microseconds in the future.  It will
 * internally add it to the timer management data structures, thus
 * spu_timer_add() is automatically called.
 */
VOID spu_timer_schedule(struct spu_timer_list *timer, UINT32 seconds, UINT32 microseconds) {
	my_real_timer_t current_time;
	UINT64 total_umicroseconds = 0;
	UINT64 total_seconds = 0;

	if (spu_gettime(&current_time)) {
		log_msg(EROR_LOG, CSL, "spu_gettime failed!\n");
		return;
	}

	total_umicroseconds = current_time.tv_usec + microseconds * 1000;

	total_seconds = current_time.tv_sec + seconds + total_umicroseconds / (1000 * 1000);

	total_umicroseconds = total_umicroseconds % (1000 * 1000);

	timer->timeout_flag = TIME_OUT_NORMAL;

	if (total_seconds >= MAX_SPU_TIME_SEC) {
		total_seconds -= MAX_SPU_TIME_SEC;
		timer->timeout_flag = TIME_OUT_LOOP;
	}

	timer->timeout.tv_sec = total_seconds;
	timer->timeout.tv_usec = total_umicroseconds;

	spu_timer_add(timer);
}

/*! \brief delete a timer from timer management
 *  \param[in] timer the to-be-deleted timer
 *
 * This function can be used to delete a previously added/scheduled
 * timer from the timer management code.
 */
VOID spu_timer_del(struct spu_timer_list *timer) {
	if (timer->active) {
		timer->active = 0;
		rb_erase(&timer->node, &timer->timer_p->root);
		/* make sure this is not already scheduled for removal. */
		if (!list_empty(&timer->list))
			list_del_init_only_entry(&timer->list);
	}
}

/*! \brief check if given timer is still pending
 *  \param[in] timer the to-be-checked timer
 *  \return 1 if pending, 0 otherwise
 *
 * This function can be used to determine whether a given timer
 * has alredy expired (returns 0) or is still pending (returns 1)
 */
INT32 spu_timer_pending(struct spu_timer_list *timer) {
	return timer->active;
}

/*! \brief compute the remaining time of a timer
 *  \param[in] timer the to-be-checked timer
 *  \param[in] the current time (NULL if not known)
 *  \param[out] remaining remaining time until timer fires
 *  \return OK if timer has not expired yet, -1 if it has
 *
 *  This function can be used to determine the amount of time
 *  remaining until the expiration of the timer.
 */
INT32 spu_timer_remaining(const struct spu_timer_list *timer, const my_real_timer_t *now,
		my_real_timer_t *remaining) {
	my_real_timer_t current_time;

	if (!now) {
		spu_gettime(&current_time);
		now = &current_time;
	}

	timersub(&timer->timeout, &current_time, remaining);

	if (remaining->tv_sec < 0) {

		log_msg(EROR_LOG, CSL, "remaining->tv_sec < 0 \n");
		return ERROR;
	}

	return OK;
}

/*! \brief Determine time between now and the nearest timer
 *  \returns pointer to timeval of nearest timer, NULL if there is none
 *
 * if we have a nearest time return the delta between the current
 * time and the time of the nearest timer.
 * If the nearest timer timed out return NULL and then we will
 * dispatch everything after the select
 */
my_real_timer_t *spu_timers_nearest(spu_timer_t* timer_p) {
	/* nearest_p is exactly what we need already: NULL if nothing is
	 * waiting, {0,0} if we must dispatch immediately, and the correct
	 * delay if we need to wait */
	return timer_p->nearest_p;
}

static VOID update_nearest(my_real_timer_t *cand, my_real_timer_t *current,
		spu_timer_t* timer_p) {
	if (cand->tv_sec != LONG_MAX) {
		if (timercmp(cand, current, >))
			timersub(cand, current, &timer_p->nearest);
		else {
			/* loop again inmediately */
			timer_p->nearest.tv_sec = 0;
			timer_p->nearest.tv_usec = 0;
		}
		timer_p->nearest_p = (my_real_timer_t *) &nearest;
	} else {
		timer_p->nearest_p = NULL;
	}
}

/*! \brief Find the nearest time and update nearest_p */
VOID spu_timers_prepare(spu_timer_t *timer_p) {
	struct rb_node *node = NULL;
	my_real_timer_t current;

	spu_gettime(&current);

	node = rb_first(&timer_p->root);
	if (node) {
		struct spu_timer_list *this;
		this = container_of(node, struct spu_timer_list, node);
		update_nearest(&this->timeout, &current, timer_p);
	} else {
		timer_p->nearest_p = NULL;
	}
}

/*! \brief fire all timers... and remove them */
INT32 spu_timers_update(spu_timer_t *timer_p) {
	my_real_timer_t current_time;
	struct rb_node *node = NULL;
	struct list_creat_head timer_eviction_list;
	struct spu_timer_list *this = NULL;
	INT32 work = 0;
	if (spu_gettime(&current_time)) {
		log_msg(EROR_LOG, CSL, "spu_gettime failed!\n");
		return ERROR;
	}

	INIT_LIST_HEAD(&timer_eviction_list);
	for (node = rb_first(&timer_p->root); node; node = rb_next(node)) {
		this = container_of(node, struct spu_timer_list, node);
		if (this->timeout_flag == TIME_OUT_LOOP) {
			if (current_time.tv_sec <= PROTECT_TIME_OUT) {
				this->timeout_flag = TIME_OUT_NORMAL;
				if (timercmp(&this->timeout, &current_time, >)) {
					break;
				}
			} else {
				break;
			}
		}

		if (timercmp(&this->timeout, &current_time, >)) {
			break;
		}

		list_add(&this->list, &timer_eviction_list);
	}

	/*
	 * The callbacks might mess with our list and in this case
	 * even list_for_each_entry_safe is not safe to use. To allow
	 * spu_timer_del to be called from within the callback we need
	 * to restart the iteration for each element scheduled for removal.
	 *
	 * The problematic scenario is the following: Given two timers A
	 * and B that have expired at the same time. Thus, they are both
	 * in the eviction list in this order: A, then B. If we remove
	 * timer B from the A's callback, we continue with B in the next
	 * iteration step, leading to an access-after-release.
	 */
	restart: list_for_each_entry(this, &timer_eviction_list, list) {
		spu_timer_del(this);
		this->cb(this->data);
		work += 1;
		goto restart;
	}

	return work;
}

INT32 spu_timers_check(spu_timer_t *timer_p) {
	struct rb_node *node = NULL;
	INT32 i = 0;

	for (node = rb_first(&timer_p->root); node; node = rb_next(node)) {
		i++;
	}
	return i;
}

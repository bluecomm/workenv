#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "spu_select.h"
#include "spu_list.h"
#include <sys/socket.h>
#include <sys/select.h>
#include "spu_log.h"
#include "spu_timer.h"
#include "spu_common.h"
#include <unistd.h>

static process_info_t process_info;
static struct list_creat_head rd_fds;

struct list_creat_head*
get_select_fds(VOID) {
	rd_fds.next = &rd_fds;
	rd_fds.prev = &rd_fds;

	return &rd_fds;
}

INT32 get_select_fds_num(VOID) {
	return process_info.select_rd_fds_num;
}

INT32 spu_fd_register(INT32 fd, VOID *cb_data, INT32 (*read_cb)(VOID* cb_data),
		CHAR *fd_name_p) {
	spu_fd_t *fd_p = NULL;

	if ((NULL == read_cb) || (NULL == fd_name_p)) {
		log_msg(EROR_LOG, TNML, "input para is NULL!\n");
		close(fd);
#ifdef SPU_DEBUG
		log_msg(WARNING_LOG, TNML,
				"Register a new fd [%s](%d) UnSuccessfully!Now select fds num is %d!\n",
				fd_name_p, fd, get_select_fds_num());
#endif
		return ERROR;
	}

	if (NULL == (fd_p = (spu_fd_t *) spu_calloc(1,sizeof(spu_fd_t)))) {
		log_msg(EROR_LOG, TNML, "memory is out!\n");
		close(fd);
#ifdef SPU_DEBUG
		log_msg(WARNING_LOG, TNML,
				"Register a new fd [%s](%d) UnSuccessfully!Now select fds num is %d!\n",
				fd_name_p, fd, get_select_fds_num());
#endif
		return ERROR;
	}

	fd_p->cb = read_cb;
	fd_p->cb_data = cb_data;
	fd_p->fd = fd;
	fd_p->fd_name = fd_name_p;

	list_add_tail(&fd_p->list, get_rd_fds());

	process_info.select_rd_fds_num++;

//#ifdef SPU_DEBUG
	log_msg(WARNING_LOG, TNML,
			"Register a new fd [%s](%d) Successfully!Now select fds num is %d!\n",
			fd_name_p, fd, get_select_fds_num());
//#endif

	return OK;
}

INT32 spu_fd_unregister(INT32 fd) {
	spu_fd_t *ufd_p = NULL, *ufd_p1 = NULL;
	struct list_creat_head *rd_fds_p = NULL;

	if (NULL == (rd_fds_p = get_rd_fds())) {
		log_msg(EROR_LOG, CSL, "get rd fds failed!\n");
#ifdef SPU_DEBUG
		log_msg(WARNING_LOG, TNML,
				"Unregister a fd [%s](%d) UnSuccessfully!Now select fds num is %d!\n",
				ufd_p->fd_name, fd, get_select_fds_num());
#endif
		return ERROR;
	}

	list_for_each_entry_safe(ufd_p,ufd_p1, rd_fds_p, list) {
		if (fd == ufd_p->fd) {
			break;
		}
	}

	if (ufd_p->list.next == rd_fds_p->next) {
		log_msg(EROR_LOG, CSL, "the fd %d is not in registered fds!!!!!\n ", fd);
#ifdef SPU_DEBUG
		log_msg(WARNING_LOG, TNML,
				"Unregister a fd [%s](%d) UnSuccessfully!Now select fds num is %d!\n",
				ufd_p->fd_name, fd, get_select_fds_num());
#endif
		return ERROR;
	}

	process_info.select_rd_fds_num--;

#ifdef SPU_DEBUG
	log_msg(WARNING_LOG, TNML,
			"Unregister a fd [%s](%d) Successfully!Now select fds num is %d!\n",
			ufd_p->fd_name, fd, get_select_fds_num());
#endif

	list_del(&ufd_p->list, rd_fds_p);
	close(ufd_p->fd);
	spu_free(ufd_p);

	return OK;
}

INT32 create_process_info(process_id_et process_id) {

	if ((process_id < PRO_CPPL0) || (process_id >= PROCESS_MAX)) {
		log_msg(EROR_LOG, CSL, "process id is invalid %d!\n", process_id);
		return ERROR;
	}

	memset(&process_info, 0, sizeof(process_info_t));

	process_info.process_id = process_id;
	if (NULL == (process_info.read_fds_p = get_select_fds())) {
		log_msg(EROR_LOG, CSL, "get select fds_p failed!\n");
		return ERROR;
	}

	/*create timer*/
	if (NULL == (process_info.spu_timer = get_spu_timer("spu"))) {
		log_msg(EROR_LOG, CSL, "get spu timer error\n");
		return ERROR;
	}

	return OK;
}

INT32 get_process_id(VOID) {
	return process_info.process_id;
}

struct list_creat_head *
get_rd_fds(VOID) {
	return process_info.read_fds_p;
}

spu_timer_t *
get_select_timer(VOID) {
	return process_info.spu_timer;
}

void wait_for_task(UINT32 timer,UINT32 socket)
{

        const UINT32 sleeptimes = 50;
        INT32 sm ;

        sm = sleeptimes - timer - socket;


        if(sm > 0){
                usleep(sm);
        }
}

/*! \brief select main loop integration
 *  \param[in] polling should we pollonly (1) or block on select (0)
 */

INT32 spu_select_main(INT32 polling, spu_timer_t* timer_p, struct list_creat_head* fds_p) {
	spu_fd_t *rfd = NULL, *tmp = NULL;
	fd_set readset;
	spu_fd_t *read_fd_pa[MAX_FD_NUM] = { NULL };
	INT32 read_fd_num = 0;
	INT32 max_fd = 0;
	INT32 loop = 0;
	struct timeval no_time = { 0, 0 };
	if ((NULL == timer_p) || (NULL == fds_p)) {
		log_msg(EROR_LOG, CSL, "input is NULL!\n");
		return ERROR;
	}

	spu_timers_check(timer_p);
	if (!polling)
		spu_timers_prepare(timer_p);

	FD_ZERO(&readset);
	/* prepare read and write fds_pets */list_for_each_entry_safe(rfd, tmp,fds_p, list) {
		FD_SET(rfd->fd, &readset);
		if (rfd->fd > max_fd) {
			max_fd = rfd->fd;
		}
		read_fd_pa[read_fd_num++] = rfd;
	}

	/* fire timer_ps */
	int timer = spu_timers_update(timer_p);

	if (select(max_fd + 1, &readset, NULL, NULL, polling ? &no_time : NULL) < 0) {
		log_msg(EROR_LOG, CSL, "select faild !\n");
		return ERROR;
	}

	int fd = 0;
	for (loop = 0; loop < read_fd_num; loop++) {
		if (FD_ISSET(read_fd_pa[loop]->fd, &readset)) {
			fd ++;
			if (read_fd_pa[loop]->cb(read_fd_pa[loop]->cb_data)) {
				log_msg(EROR_LOG, CSL, "cb faild!\n");
				return ERROR;
			}
		}
	}
	wait_for_task(timer,fd);
	return OK;
}


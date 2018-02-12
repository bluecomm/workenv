#ifndef _SPU_SELECT_H_
#define _SPU_SELECT_H_
#include "spu_list.h"
#include "spu_timer.h"

#define MAX_FD_NUM 50

/*! \brief Structure representing a file dsecriptor */
typedef struct spu_fd_s {
	/*! linked list for internal management */
	struct list_creat_head list;
	/*! actual operating-system level file decriptor */
	INT32 fd;
	CHAR *fd_name;
	/*! call-back function to be called once file descriptor becomes
	 * available */
	INT32 (*cb)(VOID *cb_data);
	/*! data pointer passed through to call-back function */
	VOID *cb_data;
} spu_fd_t;

typedef struct process_info_s {
	process_id_et process_id;
	struct list_creat_head* read_fds_p;
	spu_timer_t *spu_timer;
	INT32 select_rd_fds_num;
} process_info_t;

struct list_creat_head* get_select_fds();

INT32 spu_fd_register(INT32 fd, VOID *cb_data, INT32 (*read_cb)(VOID* cb_data),
		CHAR *fd_name_p);

INT32 spu_fd_unregister(INT32 fd);

INT32 create_process_info(process_id_et process_id);

INT32 get_process_id(VOID);
struct list_creat_head * get_rd_fds(VOID);

spu_timer_t * get_select_timer(VOID);

INT32 spu_select_main(INT32 polling, spu_timer_t* timer, struct list_creat_head* fds);

INT32 get_select_fds_num(VOID);

#endif /* _SPU_SELECT_H */

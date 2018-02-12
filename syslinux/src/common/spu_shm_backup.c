/*******************************************************************************
 ** 
 ** Copyright (c) 2014-2015 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: shmem_backup.c
 ** Description: backup for spu.
 **
 ** Current Version: 0.0
 ** Revision: 0.0.0.0
 ** Author: jinjie(jinjie@ict.ac.cn)
 ** Date: 2015.04.14
 **
 ********************************************************************************/

#include "spu_shm_backup.h"
#include "spu_log.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

static spu_shm_info_t spu_shm_mgmt[PROCESS_MAX];

/*init share memory for spu backup*/
INT32 init_shm(UINT8 pro_id, UINT8 *device_name, INT32 open_flag, INT32 page_cnt) {

	if ((NULL == device_name) || (pro_id >= PROCESS_MAX)) {
		log_msg(EROR_LOG, CSL, "device_name is NULL or pro id %d is invalid!\n", pro_id);
		return ERROR;
	}

	/*open a device for share mem*/
	if ((spu_shm_mgmt[pro_id].dev_fd = open((const CHAR *) device_name, open_flag)) < 0) {
		log_msg(EROR_LOG, CSL, "open device %s failed!\n", device_name);
		return ERROR;
	}

	/*mmap*/
	spu_shm_mgmt[pro_id].addr_size = sysconf(_SC_PAGESIZE) * page_cnt;

	lseek(spu_shm_mgmt[pro_id].dev_fd, spu_shm_mgmt[pro_id].addr_size, SEEK_SET);
	write(spu_shm_mgmt[pro_id].dev_fd, INIT_SHM_DEV_DATA, INIT_SHM_DEV_DATA_LEN);

	if ((spu_shm_mgmt[pro_id].start_addr_p = (VOID *) mmap(NULL,
			spu_shm_mgmt[pro_id].addr_size, PROT_READ | PROT_WRITE, MAP_SHARED,
			spu_shm_mgmt[pro_id].dev_fd, 0)) == MAP_FAILED) {
		log_msg(EROR_LOG, CSL, "mmap failed!\n");
		close(spu_shm_mgmt[pro_id].dev_fd);
		return ERROR;
	}

#ifdef SPU_DEBUG
	log_msg(SUMMARY_LOG, CSL,
			"share memory mmap successfully!device name is %s|start addr is %p|memory size is page size(%d)X%d = %d|\n",
			device_name, spu_shm_mgmt[pro_id].start_addr_p, sysconf(_SC_PAGESIZE), page_cnt, spu_shm_mgmt[pro_id].addr_size);
#endif

	return OK;
}

/*delete and free all shm resource*/
INT32 clean_shm(UINT8 pro_id) {
	if (pro_id >= PROCESS_MAX) {
		log_msg(EROR_LOG, CSL, "pro id %d is invalid!\n", pro_id);
		return ERROR;
	}

	if (munmap(spu_shm_mgmt[pro_id].start_addr_p, spu_shm_mgmt[pro_id].addr_size) < 0) {
		log_msg(EROR_LOG, CSL, "pro id %d  start addr %p addr size %d\n",
				pro_id, spu_shm_mgmt[pro_id].start_addr_p, spu_shm_mgmt[pro_id].addr_size);
		return ERROR;
	}

	return OK;
}

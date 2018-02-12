/******************************************************************************* **
 ** Copyright (c) 2014-2050 PANDA(NJ) && ICT/CAS.
 ** All rights reserved.
 **
 ** File name: spu_shm_backup.h.
 ** Description: spu share mem for backup.
 ** Vision: v1.0
 ** Author: jinjie(jinjie@ict.ac.cn)
 ** Date: 2015-4-14 pm2:36:50
 **
 ******************************************************************************/

#ifndef SPU_SHM_BACKUP_H_
#define SPU_SHM_BACKUP_H_

#include "spu_common.h"
#include "spu_type.h"
#include "layer_interface.h"

#define CPPL0_SHM_DEVICE_NAME "/tmp/backup/cppl0_shm_dev"
#define CPPL1_SHM_DEVICE_NAME "/tmp/backup/cppl1_shm_dev"
#define CPPL2_SHM_DEVICE_NAME "/tmp/backup/cppl2_shm_dev"
#define CPPL3_SHM_DEVICE_NAME "/tmp/backup/cppl3_shm_dev"

#define TNML_SHM_DEVICE_NAME "/tmp/backup/tnml_shm_dev"
#define RRML_SHM_DEVICE_NAME "/tmp/backup/rrml_shm_dev"
#define CCCHSCHL_SHM_DEVICE_NAME "/tmp/backup/ccchschl_shm_dev"

#define INIT_SHM_DEV_DATA "spu"
#define INIT_SHM_DEV_DATA_LEN 3

#define OPEN_SHM_DEV_FLAG O_RDWR | O_NONBLOCK | O_CREAT | O_TRUNC | O_DIRECT

typedef struct spu_shm_info_s {
	INT32 dev_fd;
	VOID *start_addr_p;
	UINT32 addr_size;
} spu_shm_info_t;

INT32 init_shm(UINT8 pro_id, UINT8 *device_name, INT32 open_flag, INT32 page_cnt);

INT32 clean_shm(UINT8 pro_id);

#endif /* SPU_SHM_BACKUP_H_ */

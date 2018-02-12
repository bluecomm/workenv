/*****************************************************************************
 ** Copyright (c) 2014-2050 PANDA(NJ) && ICT/CAS.
 ** All rights reserved.
 **
 ** File name: cppl_main.c.
 ** Description: cppl main .
 ** Vision: v1.0
 ** Author: hexiwei(hexiwei@ict.ac.cn)
 ** Date: 2015-01-30
 **
 ******************************************************************************/

#ifndef _SPU_COMMON_FUN_H_
#define _SPU_COMMON_FUN_H_
#include "rrc_core.h"
#include "spu_dpu_interface.h"
#include "spu_data_core.h"

#ifdef SPU_DEBUG
extern struct timeval time1, time2, time3, time4, time5;
#endif

CHAR *spu_layer_msg_type_value_to_string(spu_layer_msg_type_et spu_layer_msg_type);
CHAR *process_id_value_to_string(process_id_et process_id);
CHAR *dpu_spu_confirm_msg_process_id_value_to_string(
		dpu_spu_confirm_msg_process_id_et process_id);
CHAR *cppl_rrml_msg_type_value_to_string(msg_type_et msg_type);
CHAR *mes_event_value_to_string(mes_event_et mes_event);
CHAR *mes_state_value_to_string(mes_state_et mes_state);
CHAR *dpu_to_spu_msg_union_type_value_to_string(dpu_to_spu_msg_union_type_et msg_type);
CHAR *reject_cause_value_to_string(rej_cause_et rej_cause);
CHAR *rrc_estab_cause_value_to_string(rrc_estab_cause_et rrc_estab_cause);
INT32 log_tnml2cppl_nbap_msg(spu_layer_msg_t *cppl_in_msg_p);
INT32 log_rrml2cppl_layer_msg(spu_layer_msg_t *cppl_in_msg_p);
INT32 log_tnml2cppl_dpu_layer_msg(spu_layer_msg_t *cppl_in_msg_p,
		dpu_to_spu_msg_t *dpu_to_spu_msg_p);
#endif

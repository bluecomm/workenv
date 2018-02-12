/*******************************************************************************
 **
 ** Copyright (c) 2013-2014 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: spu_log.h
 ** Description:
 **
 ** Current Version: 0.0
 ** Revision: 0.0.0.0
 ** Author: jinjie(jinjie@ict.ac.cn)
 ** Date: 2014-07-18
 **
 ******************************************************************************/

#ifndef _SPU_LOG_H_
#define _SPU_LOG_H_

/* Dependencies ------------------------------------------------------------- */
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "spu_type.h"

/* Constants ---------------------------------------------------------------- */

/* Types -------------------------------------------------------------------- */

#define LOG_FILE_CPPL0 "/tmp/cppl0.log"
#define LOG_FILE_CPPL1 "/tmp/cppl1.log"
#define LOG_FILE_CPPL2 "/tmp/cppl2.log"
#define LOG_FILE_CPPL3 "/tmp/cppl3.log"

#define LOG_FILE_TNML "/tmp/tnml.log"
#define LOG_FILE_SMML "/tmp/smml.log"
#define LOG_FILE_CCCHSCHL "/tmp/ccchschl.log"
#define LOG_FILE_RRML "/tmp/rrml.log"

/*define foreground colour*/
typedef enum foreground_e {
	FG_BLACK = 30,
	FG_RED,
	FG_GREEN,
	FG_YELLOW,
	FG_BLUE,
	FG_PURPLE,
	FG_CYAN,
	FG_WHITE,
	FG_MAX
} foreground_et;

/* describe the mode which module will be working in*/
typedef enum logfile_e {
	WRITE2STDOUT = 0, /* write the output to stdout*/
	WRITE2FILE, /* write the output to file*/
	FILEMAX
} logfile_et;

/* log level*/
typedef enum loglevel_e {
	EROR_LOG = 0,/* used when fatal error occurs*/
	DEBUG_LOG,/* used when debug occurs*/
	WARNING_LOG, /* used when un-nomarl */
	SUMMARY_LOG, /* summary info,Normal, but significant events */
	INFO_LOG,/*run info*/
	LOG_MAX
} loglevel_et;

typedef enum log_degree_e {
	LOG_SIMPLE = 0, LOG_COMPLETE, LOG_DG_MAX
} log_degree_et;

/* module names, users can output the specific module debug info*/
/* while omit debug info of other modules', by specify the module*/
/* name */
typedef enum log_module_e {
	CPPL, TNML, SMML, CCCHSCHL, CSL, RRML,
	/* need to be completed*/
	LOG_MAX_MODULE_NAME
} log_module_et;

/* Macros ------------------------------------------------------------------- */
#ifdef USE_LOG
#define log_msg(level, module, fmt, arg...) \
		spu_log_msg(level, module, #module, __FILE__, __LINE__, __func__, fmt, ##arg)
/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */

/*******************************************************************************
 * init the log module 
 * Input:
 *		style: WRITE2FILE or WRITE2STDOUT
 *		current_level: EROR_LOG, WARRNING_LOG or DEBUG_LOG, indicating the lowest
 *					level that the module will excute
 * Output:
 *		None;
 * Return:
 *		0:success;
 *		-1:failed
 ******************************************************************************/
INT32 log_init(UINT8 log_file, UINT8 log_degree, UINT8 log_level);
/*******************************************************************************
 * cleanup the log module ,release the resource
 * Input:
 *		None
 * Output:
 *		None;
 * Return:
 *		0:success;
 *		-1:failed
 ******************************************************************************/
INT32 log_close();
/*******************************************************************************
 * turn on the log print of one module 
 * Input:
 *		module :name of module
 * Output:
 *		None;
 * Return:
 *		None;
 ******************************************************************************/
VOID log_open_module(UINT8 module);
/*******************************************************************************
 * turn off the log print of one module 
 * Input:
 *		module :name of module
 * Output:
 *		None;
 * Return:
 *		None;
 ******************************************************************************/
VOID log_close_module(UINT8 module);

INT32 spu_log_msg(UINT8 level, UINT8 module, const CHAR *mdstr, const CHAR *file,
		INT32 lineno, const CHAR *func, const CHAR *fmt, ...);
#else
#define log_msg(level, module, fmt, arg...)
#endif
#endif /* SPU_LOG_H */

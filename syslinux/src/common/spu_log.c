/*******************************************************************************
 **
 ** Copyright (c) 2013-2014 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: spu_log.c
 ** Description:
 **
 ** Current Version: 0.0
 ** Revision: 0.0.0.0
 ** Author: jinjie(jinjie@ict.ac.cn)
 ** Date: 2014-07ERROR8
 **
 ******************************************************************************/
/* Dependencies ------------------------------------------------------------- */
#ifdef USE_LOG
#include <stdarg.h>
#include "spu_log.h"
#include "spu_common.h"
#include "spu_select.h"

/* Constants ---------------------------------------------------------------- */

/* Types -------------------------------------------------------------------- */

/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */
static UINT8 g_log_level; /* lowest level that the module will excute*/
static UINT8 g_log_degree; /* output simple debug info or complete debug info*/
static FILE *g_log_file; /* output file pointor */
/* semaphore used to make sure only one thread use g_log_file at a time*/

/* Indicate whose log message will be print*/
/* one bit indicates one module*/
static UINT64 g_module_select = 0;

const CHAR *log_level_name[LOG_MAX] = { "EROR_LOG", "DEBUG_LOG", "WARNING_LOG",
		"SUMMARY_LOG", "INFO_LOG" };

/* Functions ---------------------------------------------------------------- */

/*******************************************************************************
 * init the log module 
 * Input:
 *		style: WRITE2FILE or WRITE2STDOUT
 *		format: LOG_SIMPLE or LOG_COMPLETE
 *		current_level: LOG_ERR, LOG_WARRNING or DEBUG_LOG, indicating the lowest
 *					level that the module will excute
 * Output:
 *		None;
 * Return:
 *		0:success;
 *		ERROR:failed
 ******************************************************************************/
INT32 log_init(UINT8 log_file, UINT8 log_degree, UINT8 log_level) {
	INT32 process_id = PROCESS_MAX;

	if (log_level < LOG_MAX) {
		g_log_level = log_level;
	} else {
		printf("LOG module: current_level error %d\n", log_level);
		return ERROR;
	}
	if (log_degree < LOG_DG_MAX) {
		g_log_degree = log_degree;
	} else {
		printf("LOG module: format error %d\n", log_degree);
		return ERROR;
	}
	if (log_file == WRITE2STDOUT) {
		g_log_file = stdout;
	} else {
		process_id = get_process_id();
		switch (process_id) {
		case PRO_CPPL0:
			if (NULL != (g_log_file = fopen(LOG_FILE_CPPL0, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		case PRO_CPPL1:
			if (NULL != (g_log_file = fopen(LOG_FILE_CPPL1, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		case PRO_CPPL2:
			if (NULL != (g_log_file = fopen(LOG_FILE_CPPL2, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		case PRO_CPPL3:
			if (NULL != (g_log_file = fopen(LOG_FILE_CPPL3, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		case PRO_RRML:
			if (NULL != (g_log_file = fopen(LOG_FILE_RRML, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		case PRO_TNML:
			if (NULL != (g_log_file = fopen(LOG_FILE_TNML, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		case PRO_CCCHSCHL:
			if (NULL != (g_log_file = fopen(LOG_FILE_CCCHSCHL, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		case PRO_SMML:
			if (NULL != (g_log_file = fopen(LOG_FILE_SMML, "w"))) {
				printf("LOG module: open file error\n");
				return ERROR;
			}
			break;
		default:
			printf("process id is %d invalid\n", process_id);
			return ERROR;
			break;
		}
	}

	return OK;
}
/*******************************************************************************
 * cleanup the log module ,release the resource
 * Input:
 *		None
 * Output:
 *		None;
 * Return:
 *		0:success;
 *		ERROR:failed
 ******************************************************************************/
INT32 log_close(VOID) {
	INT32 i = 0;

	if (g_log_file != stdout) {
		fclose(g_log_file);
		g_log_file = NULL;
	}

	g_log_level = LOG_MAX;
	g_log_degree = LOG_DG_MAX;

	for (i = CPPL; i < LOG_MAX_MODULE_NAME; i++)
		log_close_module(i);

	return OK;
}
/*******************************************************************************
 * turn on the log print of one module 
 * Input:
 *		module :name of module
 * Output:
 *		None;
 * Return:
 *		None;
 ******************************************************************************/
VOID log_open_module(UINT8 module) {

	if (module < LOG_MAX_MODULE_NAME) {
		g_module_select |= 1 << module;
	} else {
		printf("LOG module , module name invalid %d\n", module);
	}
}
/*******************************************************************************
 * turn off the log print of one module 
 * Input:
 *		module :name of module
 * Output:
 *		None;
 * Return:
 *		None;
 ******************************************************************************/
VOID log_close_module(UINT8 module) {

	if (module < LOG_MAX_MODULE_NAME) {
		g_module_select &= ~(1 << module);
	} else {
		printf("LOG module , module name invalid %d\n", module);
	}
}

/*******************************************************************************
 * output the debug information, this function won't be used directly by users
 * Input:
 *		level: the output level
 *		*fmt: varable argument
 *		...: optional argument
 * Output
 *		None;
 * Return:
 *		0:success;
 *		ERROR:failed
 ******************************************************************************/
INT32 spu_log_msg(UINT8 level, UINT8 module, const CHAR *mdstr, const CHAR *file,
		INT32 lineno, const CHAR *func, const CHAR *fmt, ...) {
	CHAR *title = NULL;
	INT32 color = FG_BLACK;
	FILE *log_file = NULL;

	if (level <= g_log_level) {
		if (g_module_select & (1 << module)) {
			switch (level) {
			case DEBUG_LOG:
				title = "DEBUG";
				color = FG_BLACK;
				break;
			case EROR_LOG:
				title = "ERROR";
				color = FG_RED;
				break;
			case WARNING_LOG:
				title = "WARNING";
				color = FG_PURPLE;
				break;
			case SUMMARY_LOG:
				title = "SUMMARY";
				color = FG_BLUE;
				break;
			case INFO_LOG:
				title = "INFO";
				color = FG_YELLOW;
				break;
			}

			log_file = g_log_file;

			if (g_log_degree == LOG_COMPLETE) {
				if (log_file == stdout) {
					fprintf(log_file, "\033[0m");
					fprintf(log_file, "\033[1;%dm", color);
				}
				if ((EROR_LOG == level) || (DEBUG_LOG == level)) {
					fprintf(log_file, "[%s|%s]%s:%d in func %s:\n", title, mdstr, file,
							lineno, func);
				} else {
					fprintf(log_file, "[%s|%s]:", title, mdstr);
				}
			}
			va_list ap;
			va_start(ap, fmt);
			vfprintf(log_file, fmt, ap);
			if (log_file == stdout)
				fprintf(log_file, "\033[0m");
			va_end(ap);

			fflush(log_file);
		}
	}
	return OK;
}
#endif

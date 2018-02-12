#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#define NAME_MAX 16
#define SQL_MAX 256
#define TELEPHONE_MAX 16

char *table_name = "test_table";
char *db_name = "test2.db";

#define SQLITE3_EXEC(db, sql, callback, parg, zErr, rc) \
	printf("[SQL]: %s\n", sql);\
	zErr = NULL;\
	rc = sqlite3_exec(db, sql, callback, parg, &zErr);\
	if (rc != SQLITE_OK) {\
		if (zErr != NULL) {\
			fprintf(stderr, "SQL error: %s\n", zErr);\
			sqlite3_free(zErr);\
		}\
	}

#define PRINT_TABLE(row_indx, nrows, col_indx, ncols, result) \
		printf("rows=%d,cols=%d\n", nrows, ncols);\
			for (row_indx = 0; row_indx <= nrows; row_indx++) {\
				for (col_indx = 0; col_indx < ncols; col_indx++) {\
					printf("%-20s", result[row_indx * ncols + col_indx]);\
				}\
				printf("\n");\
			}

void print_options() {
	printf("\noptions: \n");
	printf("\t0\t exit\n"
			"\t11\t create table\n"
			"\t12\t insert a row\n"
			"\t13\t update an existing row\n"
			"\t14\t update part of table\n"
			"\t15\t update telephone\n"
			"\t21\t delete a row in table\n"
			"\t23\t delete table\n"
			"\t31\t select row\n"
			"\t32\t show table\n");
	return;
}
int test1() {
	int opt;

	sqlite3 *db;
	char *zErr;
	int rc;
	char sql[SQL_MAX];
	char **result;
	int nrows, ncols;
	int i, j;
	//char *table_name = "test_table";

	int id;
	char name[NAME_MAX];
	char sex;
	char telephone[TELEPHONE_MAX];
	char sure = 0;

	/*****************************************************************/
	/* open db */
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	/*****************************************************************/
	/* create table */
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "create table if not exists %s("
			"id integer primary key,"
			"name text, "
			"sex text, "
			"telephone text)", table_name);
	SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc);

	while (1) {
		print_options();
		printf("\noption: ");
		scanf("%d", &opt);
		switch (opt) {

		case 0:
			/*****************************************************************/
			/* exit */
			printf("exit\n");
			goto end_of_test;
			break;
		case 11:
			/*****************************************************************/
			/* create table */
			memset(sql, 0, sizeof(sql));
			sprintf(sql, "create table if not exists %s("
					"id integer primary key,"
					"name text, "
					"sex text, "
					"telephone text)", table_name);
			SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			;
			break;
		case 12:
			/*****************************************************************/
			/* insert row*/
			memset(name, 0, sizeof(name));
			memset(sql, 0, sizeof(sql));
			memset(telephone, 0, sizeof(telephone));
			printf("input values: ");
			scanf("%d %s %c %s", &id, name, &sex, telephone);
			sprintf(sql,
					"insert into %s (id,name,sex,telephone) values (%d, '%s','%c', '%s')",
					table_name, id, name, sex, telephone);
			SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			;
			break;
		case 13:
			/*****************************************************************/
			/* update an existing row*/
			memset(name, 0, sizeof(name));
			memset(sql, 0, sizeof(sql));
			memset(telephone, 0, sizeof(telephone));
			printf("input values: ");
			//printf("%s, %d\n", __FILE__, __LINE__);
			scanf("%d %s %c %s", &id, name, &sex, telephone);
			//printf("%s, %d\n", __FILE__, __LINE__);
			sprintf(sql,
					"update %s set name='%s', sex='%c', telephone='%s' where id = %d",
					table_name, name, sex, telephone, id);
			//printf("%s, %d\n", __FILE__, __LINE__);
			SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			;
			//printf("%s, %d\n", __FILE__, __LINE__);
			break;
		case 14:
			/*****************************************************************/
			/* insert row*/
			memset(sql, 0, sizeof(sql));
			memset(telephone, 0, sizeof(telephone));
			printf("input values: ");
			scanf("%d %s", &id, telephone);
			sprintf(sql, "replace into %s (id,telephone) values (%d, '%s')",
					table_name, id, telephone);
			SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			;
			break;
		case 15:
			/*****************************************************************/
			/* insert row*/
			memset(sql, 0, sizeof(sql));
			memset(telephone, 0, sizeof(telephone));
			printf("input values: ");
			scanf("%d %s", &id, telephone);

			sprintf(sql, "select * from %s where id = %d;", table_name, id);
			rc = sqlite3_get_table(db, sql, &result, &nrows, &ncols, &zErr);
			if (rc != SQLITE_OK) {
				if (zErr != NULL) {
					fprintf(stderr, "SQL error: %s\n", zErr);
					sqlite3_free(zErr);
				}
			}
			if (nrows <= 0) {
				printf("no exist, insert\n");
				memset(sql, 0, sizeof(sql));
				sprintf(sql, "insert into %s (id,telephone) values (%d, '%s')",
						table_name, id, telephone);
				SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			} else {
				printf("exist, update\n");
				memset(sql, 0, sizeof(sql));
				sprintf(sql, "update %s set telephone = '%s' where id = %d",
						table_name, telephone, id);
				SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			}
			break;
		case 21:
			/*****************************************************************/
			/* delete a row in table */
			printf("input id: ");
			scanf("%d", &id);
			memset(sql, 0, sizeof(sql));
			sprintf(sql, "delete from %s where id = %d", table_name, id);
			SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			;
			break;
//		case 22:
//			/*****************************************************************/
//			/* dedlete all rows from table but donnot delete table */
//			memset(sql, 0, sizeof(sql));
//			sprintf(sql, "truncate %s", table_name);
//			SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
//			;
//			break;
		case 23:
			/*****************************************************************/
			/* delete table */
			printf("Delete table ?(y/n)");
			scanf("%c", &sure);
			if (sure == 'y') {
				memset(sql, 0, sizeof(sql));
				sprintf(sql, "drop table %s", table_name);
				SQLITE3_EXEC(db, sql, NULL, NULL, zErr, rc)
			}
			break;
		case 31:
			/*****************************************************************/
			/* select a row */
			printf("input id: ");
			scanf("%d", &id);
			memset(sql, 0, sizeof(sql));
			sprintf(sql, "select * from %s where id = %d;", table_name, id);
			rc = sqlite3_get_table(db, sql, &result, &nrows, &ncols, &zErr);
			/* Do something with data */
			PRINT_TABLE(i, nrows, j, ncols, result)
			;
			/* Free memory */
			sqlite3_free_table(result);
			;
			break;
		case 32:
			/*****************************************************************/
			/* show table */
			memset(sql, 0, sizeof(sql));
			sprintf(sql, "select * from %s", table_name);
			rc = sqlite3_get_table(db, sql, &result, &nrows, &ncols, &zErr);
			/* Do something with data */
			PRINT_TABLE(i, nrows, j, ncols, result)
			;
			/* Free memory */
			sqlite3_free_table(result);
			break;
		default:
			/*****************************************************************/
			/* do nothing */
			printf("unknown option\n");
			break;
		}
	}
	end_of_test: sqlite3_close(db);

	return 0;
}

int main(int argc, char **argv) {
	test1();
	return 0;
}

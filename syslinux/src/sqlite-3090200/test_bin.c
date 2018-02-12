#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#define PRINT_TABLE(row_indx, nrows, col_indx, ncols, result) \
		printf("rows=%d,cols=%d\n", nrows, ncols);\
			for (row_indx = 0; row_indx <= nrows; row_indx++) {\
				for (col_indx = 0; col_indx < ncols; col_indx++) {\
					printf("%-20s", result[row_indx * ncols + col_indx]);\
				}\
				printf("\n");\
			}

int main(int argc, char **argv) {
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *zErr;
	int rc;
	char sql[256];
	char **result;
	int nrows, ncols;
	int i, j;

	/**************************************************************/
	rc = sqlite3_open("test_bin.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	/**************************************************************/
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "create table if not exists test_bin("
			"idx int  primary key, name varchar(16), bin_data blob)");
	printf("[SQL]: %s\n", sql);
	rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
	if (rc != SQLITE_OK) {
		if (zErr != NULL) {
			fprintf(stderr, "SQL error: %s\n", zErr);
			sqlite3_free(zErr);
		}
	}
	/**************************************************************/
	unsigned char bin_data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6,
			7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2 };
	for (i = 0; i < sizeof(bin_data); ++i) {
		printf("%x ", bin_data[i]);
	}
	printf("\n");

	for (i = 0; i < 5; ++i){
		memset(sql, 0, sizeof(sql));
		sprintf(sql, "insert into test_bin values (%d, '%d.bin',?)", i+1, i+1);
		printf("[SQL]: %s\n", sql);
		sqlite3_prepare(db, sql, -1, &stmt, NULL);
		bin_data[0] = i+16;
		sqlite3_bind_blob(stmt, 1, bin_data, sizeof(bin_data), NULL);
		sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);

	/**************************************************************/
	char buffer[sizeof(bin_data) + 1];
	memset(buffer, 0, sizeof(buffer));
	char *data;
	int data_len;

	memset(sql, 0, sizeof(sql));
	sprintf(sql, "select * from test_bin");
	printf("[SQL]: %s\n", sql);
	sqlite3_prepare(db, sql, -1, &stmt, 0);

	rc = sqlite3_step(stmt);

	ncols = sqlite3_column_count(stmt);
	for (i = 0; i < ncols; i++) {
		fprintf(stdout, "Column: name=%s, \tstorage class=%i, \tdeclared=%s\n",
				sqlite3_column_name(stmt, i), sqlite3_column_type(stmt, i),
				sqlite3_column_decltype(stmt, i));
	}
	fprintf(stdout, "\n");

	while (rc == SQLITE_ROW) {
		printf("%d", sqlite3_column_int(stmt, 0));
		printf("\t%s\t", sqlite3_column_text(stmt, 1));
		data = (char *) sqlite3_column_blob(stmt, 2);
		data_len = sqlite3_column_bytes(stmt, 2);
		//printf("data: %p, data_len: %d\n", data, data_len);
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, data, data_len);
		for (j = 0; j < data_len; ++j) {
			printf("%x ", buffer[j]);
		}
		printf("\n");
		rc = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);

	/**************************************************************/
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "select idx, name from test_bin");
	printf("[SQL]: %s\n", sql);
	rc = sqlite3_get_table(db, sql, &result, &nrows, &ncols, &zErr);
	/* Do something with data */
	PRINT_TABLE(i, nrows, j, ncols, result);
	/* Free memory */
	sqlite3_free_table(result);

	sqlite3_close(db);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"


int main(int argc, char **argv) {
	sqlite3 *db;
	char *zErr;
	int rc;
	char *sql;
	char **result;
	int nrows, ncols;
	int i, j;

	rc = sqlite3_open("test.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	sql = "create table episodes( id integer primary key,"
			"name text, cid int)";
	printf("%s\n", sql);
	rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
	if (rc != SQLITE_OK) {
		if (zErr != NULL) {
			fprintf(stderr, "SQL error: %s\n", zErr);
			sqlite3_free(zErr);
		}
	}
	sql = "insert into episodes (name,id) values ('Cinnamon Babka2',1)";
	printf("%s\n", sql);
	rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
	if (rc != SQLITE_OK) {
		if (zErr != NULL) {
			fprintf(stderr, "SQL error: %s\n", zErr);
			sqlite3_free(zErr);
		}
	}

	sql = "select * from episodes;";
	rc = sqlite3_get_table(db, sql, &result, &nrows, &ncols, &zErr);
	/* Do something with data */
	printf("rows=%d,cols=%d\n", nrows, ncols);

//	for (i = 0; i <= nrows; i++) {
//		printf("%-5s%-20s%-5s\n", result[ncols * i], result[ncols * i + 1],
//				result[ncols * i + 2]);
//	}

	for (i = 0; i <= nrows; i++) {
		for (j = 0; j < ncols; j++) {
			printf("%-20s", result[i * ncols + j]);
		}
		printf("\n");
	}
	/* Free memory */
	sqlite3_free_table(result);

	sqlite3_close(db);
	return 0;
}

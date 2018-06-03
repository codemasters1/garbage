#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

static int callback(void *reserved, int count, char **values, char **columns)
{
	int i;
	for (int i = 0; i < count; i++)
	{
		printf("%s = %s\n", columns[i], values[i] ? values[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;

	/* Open database */
	rc = sqlite3_open("test.db", &db);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else
	{
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE IF NOT EXISTS `dict` (`key` TEXT PRIMARY KEY NOT NULL);";

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
	
	return 0;
}
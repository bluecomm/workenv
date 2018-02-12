#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

time_t time_convert(char* incoming_string00) {

	//char* incoming_string00 = "2014-10-21 0824:28:30";
	char incoming_string[32];
	char* trace_ptr;
	char* token;
	time_t time_aquired;
	struct tm time_struct;

	memset(incoming_string, 0, sizeof(incoming_string));
	strcpy(incoming_string, incoming_string00);

	//year
	token = strtok_r(incoming_string, "-", &trace_ptr);
	if (token == NULL)
		goto args_incorrect;
	time_struct.tm_year = atoi(token) - 1900;//year since 1900

	//month
	token = strtok_r(NULL, "-", &trace_ptr);
	if (token == NULL)
		goto args_incorrect;
	time_struct.tm_mon = atoi(token) - 1;

	//day
	token = strtok_r(NULL, " ", &trace_ptr);
	if (token == NULL)
		goto args_incorrect;
	time_struct.tm_mday = atoi(token);

	//hour
	token = strtok_r(NULL, ":", &trace_ptr);
	if (token == NULL)
		goto args_incorrect;
	time_struct.tm_hour = (atoi(token) - 24) / 100;

	//minute
	token = strtok_r(NULL, ":", &trace_ptr);
	if (token == NULL)
		goto args_incorrect;
	time_struct.tm_min = atoi(token);

	//second
	token = strtok_r(NULL, ":", &trace_ptr);
	if (token == NULL)
		goto args_incorrect;
	time_struct.tm_sec = atoi(token);

	time_aquired = mktime(&time_struct);

	return time_aquired;

args_incorrect:
	return -1;
}

int main()
{
	char *time_str = "2014-10-22 0824:40:30";
	time_t time_now = time_convert(time_str);
	printf("time \"%s\" is %lu\nnow is %lu",
			time_str, time_now, time(NULL));
	return 0;
}

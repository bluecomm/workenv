#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define LOG_FILENAME_LEN 22
#define LOG_FILENAME_FORMAT "%4d%02d%02d_%02d%02d%02d_%05d"

int get_log_filename(char *buf, int buf_len) {

	if (buf_len < LOG_FILENAME_LEN)
		return -1;

	time_t t = time(NULL);
	struct tm *tm = localtime(&t); //gmtime(&tt);

	sprintf(buf, LOG_FILENAME_FORMAT, tm->tm_year + 1900, tm->tm_mon + 1,
			tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, getpid());

	return strlen(buf);
}

int32_t print_memory(uint8_t *msg_p, uint32_t len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		if (i % 4 == 0) {
			if (i != 0) {
				printf(" ");
				if (i % 32 == 0)
					printf("\n");
			}
		}
		printf("%02x ", msg_p[i]);
	}
	printf("\nlen = %d (0x%x)\n", len, len);

	return 0;
}

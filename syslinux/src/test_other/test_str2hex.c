#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int test_str2hex()
{
	char *s = "FF1AFE0820";
	char tmp_str[3];
	char *saveptr;
	uint32_t tmp_int;
	int i;
	
	for(i = 0; i < 5; i++)
	{
		memset(tmp_str, 0, sizeof(tmp_str));
		memcpy(tmp_str, s + i*2, 2);
		tmp_int = strtol(tmp_str, &saveptr, 16);
		printf("tmp_str: %s, tmp_int: %02x/%d\n", 
				tmp_str, tmp_int, tmp_int);
	}
	return 0;
}

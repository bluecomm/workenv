#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LENGTH 32

int test_str_split(void)
{
	char *s1 = "1,2,3,5";
	char *s2 = "1,2,3;4,5,6;7,8,9";
	
	char ss[MAX_STR_LENGTH];
	
	char *token, *subtoken;
	char *str1, *str2;
	char *saveptr1, *saveptr2;
	
	memset(ss, 0, sizeof(MAX_STR_LENGTH));
	strcpy(ss, s1);
	for(str1 = ss;;str1 = NULL)
	{
		token = strtok_r(str1,",", &saveptr1);
		if(token == NULL)
			break;
		printf("%s\n",token);
	}
	
	memset(ss, 0, sizeof(MAX_STR_LENGTH));
	strcpy(ss, s2);
	
	for(str1 = ss;;str1 = NULL)
	{
		token = strtok_r(str1,";", &saveptr1);
		if(token == NULL)
			break;
		printf("%s\n",token);
		
		for(str2 = token;;str2 = NULL)
		{
			subtoken = strtok_r(str2,",", &saveptr2);
			if(subtoken == NULL)
				break;
			printf("  -->%s\n",subtoken);
		}
	}
	
	return 0;
}

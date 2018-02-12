#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char *cmd_str = "./ipc_exe 1 2 3 4 5 > tmp.txt";
	//system(cmd_str);
	//execl();
	FILE *fp = popen(cmd_str, "w");
	//pclose(fp);
	printf("after popen()\n");
	return 0;
}

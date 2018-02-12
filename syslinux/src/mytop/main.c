#include "machine.h"
#include <stdio.h>
#include <unistd.h>

int machine_init(struct statics * statics);
void get_system_info(struct system_info *info);
//void read_one_proc_stat(pid_t pid, struct top_proc *proc);
//char *format_next_process(struct top_proc *p);

int main() {
	struct statics stat;
	machine_init(&stat);
	struct system_info info;
	get_system_info(&info);
	//struct top_proc proc;

	for (;;) {
//		printf("Used CPU:%.1f%%\n", (float) info.cpustates[0] / 10);
//		printf("Nice CPU:%.1f%%\n", (float) info.cpustates[1] / 10);
//		printf("System CPU:%.1f%%\n", (float) info.cpustates[2] / 10);
//		printf("Idle CPU:%.1f%%\n", (float) info.cpustates[3] / 10);
//
//		printf("total memroy:%d\n", info.memory[0]);
//		printf("free memroy:%d\n", info.memory[1]);
//		printf("buffers:%d\n", info.memory[2]);
//		printf("cached:%d\n", info.memory[3]);
//		printf("total swap:%d\n", info.memory[4]);
//		printf("free swap:%d\n", info.memory[5]);

		printf("----------------------------------------------------------------------------------------------------------\n");
		printf("%-12s %-12s %-12s %-12s %-22s %-22s %-20s\n", "Used CPU(%)", "Nice CPU(%)",
				"System CPU(%)", "Idle CPU(%)", "Memory(%)", "Buffer(%)", "Swap(%)");

		printf("%-12.2f %-12.2f %-13.2f %-12.2f %d/%d(%.2f) %d/%d(%.2f) %8d/%d(%.2f)\n\n",
				(float) info.cpustates[0] / 10,
				(float) info.cpustates[1] / 10,
				(float) info.cpustates[2] / 10,
				(float) info.cpustates[3] / 10,
				info.memory[1],
				info.memory[0],
				100 * (float)(info.memory[1])/(float)(info.memory[0]),
				info.memory[2],
				info.memory[3],
				100 * (float)(info.memory[2])/(float)(info.memory[3]),
				info.memory[5],
				info.memory[4],
				100 * (float)(info.memory[5])/(float)(info.memory[4]));

		sleep(2);
		//printf("..................................\n");
		get_system_info(&info);

		//read_one_proc_stat( (pid_t)7443, &proc);

		//struct top_proc *p = &proc;

		//printf("%s\n",format_next_process(p));
	}
	return 0;
}

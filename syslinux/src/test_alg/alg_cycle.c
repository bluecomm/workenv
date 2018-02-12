#include<stdio.h>
#include<stdlib.h>

#define MAX_PERSON_NUM 100

struct person_struct
{
	int id;
	struct person_struct *next;
};

int person_array(int person_number, int person_tick )
{
	int person[MAX_PERSON_NUM] = {0};
	int current_person = 0;
	int current_counter  = 1;
	int person_letf = person_number;
	
	while(person_letf > 1){	
		if(current_counter  == person_tick ){
			person[current_person]  = 1;
			person_letf--;
			current_counter  = 1;
			while(person[current_person]!=0)
					current_person = (current_person+1) % person_number;
		}else{
			current_counter++;
			current_person = (current_person+1) % person_number;
			while(person[current_person]!=0)
					current_person = (current_person+1) % person_number;
		}
	}
	return (current_person+1);
}

int person_list(int person_num, int person_tick)
{
	struct person_struct *person_head;
	struct person_struct *current_person;
	struct person_struct *tmp1;
	struct person_struct *tmp2;
	
	int person_left = person_num;
	int tick_counter = 1;
	int i;
	
	person_head = malloc(sizeof(struct person_struct));
	person_head->id = 1;
	
	tmp1 = person_head;
	for(i=1;i<person_num;i++){
		tmp2 =  malloc(sizeof(struct person_struct));
		tmp2->id = i+1;
		tmp1->next = tmp2;
		tmp1 = tmp1->next;
	}
	 tmp1->next =  person_head;
	
	current_person = tmp1 =person_head;
	//tmp1 = person_head;
	
	while(person_left > 1){
		if(tick_counter == person_tick){
			tick_counter = 1;
			tmp2 = current_person;
			tmp1->next = current_person = current_person->next;
			//tmp1->next = current_person;
			 free(tmp2);
			 tmp2 = NULL;
			 person_left --;
		}else{
			tick_counter++;
			tmp1 = current_person;
			current_person = current_person->next;
		}
	}
	
	i = current_person->id;
	free(current_person);
	current_person = NULL;
	return i;
}

int main(int argc, char *argv[])
{
	int person_tick = 7;
	int person_num = 3;

	if( person_tick <=0 || person_num <= 0|| person_tick > person_num){
		printf("bad input: tick = %d, num = %d\n", person_tick, person_num);
		return -1;
	}

	int re_array = person_array(person_num,person_tick);
	int re_list = person_list(person_num,person_tick);
	printf("The last person is %d/%d\n",re_array, re_list);

	return 0;

}



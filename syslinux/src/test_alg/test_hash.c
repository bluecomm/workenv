#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "common_types.h"
#include "uthash.h"


typedef struct test_hash_s {
	int key;
	int value;
	UT_hash_handle hh;
} test_hash_t;

int main() {

	srand(time(NULL));
	test_hash_t *hash_head = NULL;

	int i;

	for (i = 0; i < 10; ++i) {
		test_hash_t *hash_node = malloc(sizeof(test_hash_t));
		hash_node->key = i;
		hash_node->value = i * i;
		HASH_ADD_INT(hash_head, key, hash_node);

		printf("key: %d, value: %2d, addr: %p\n", hash_node->key,
				hash_node->value, hash_node);
	}

	for (i = 0; i < 10; ++i) {
		int hash_key = rand() % 10;
		test_hash_t *hash_node = NULL;
		HASH_FIND_INT(hash_head, &hash_key, hash_node);

		if (hash_node == NULL){
			printf("no find\n");
			continue;
		}
		printf("key: %d, value: %2d, addr: %p\n", hash_node->key,
				hash_node->value, hash_node);
		HASH_DEL(hash_head, hash_node);
	}

	return 0;
}

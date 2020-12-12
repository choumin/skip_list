#include <stdio.h>
#include "skip_list.h"

void test_search(skip_list *list, uint32_t key) {
	char *p = NULL;
	bool ret = skip_list_search(list, key, (void **)&p);
	if (ret) {
		printf("search %u, result: %s\n", key, p);
	} else {
		printf("%u not found!\n");
	}

}

void test_insert(skip_list *list, uint32_t key, char *p) {
	bool ret = skip_list_insert(list, key, p);
	if (ret) {
		printf("insert %u success!\n", key);
	} else {
		printf("insert %u failed!\n", key);
	}
}

void test_delete(skip_list *list, uint32_t key) {
	bool ret = skip_list_delete(list, key);
	if (ret) {
		printf("delete %u success!\n", key);
	} else {
		printf("delete %u failed!\n", key);
	}
}

void show_skip_list_level(skip_list *list, uint16_t level) {
	if (level > (list->level - 1)) {
		printf("level %u not exist, max level is %u\n", level, list->level - 1);
		return;
	}

	printf("level %u: ( ", level);
	skip_list_node *x = list->header->next_nodes[level];
	while (x) {
		printf("%u ", x->key);
		x = x->next_nodes[level];
	}

	printf(")\n");
}

int main(void) {
	float prob = 0.5;
	skip_list *list = skip_list_create(prob);
	if (list == NULL) {
		printf("create skip list failed!\n");
		return -1;
	}
	uint32_t a_key = 1;
	char *a_value = "I";

	uint32_t b_key = 2;
	char *b_value = "you";

	uint32_t c_key = 3;
	char *c_value = "she";

	uint32_t d_key = 4;
	char *d_value = "he";

	uint32_t e_key = 5;
	char *e_value = "this";

	uint32_t f_key = 6;
	char *f_value = "that";

	uint32_t g_key = 7;
	char *g_value = "it";

	test_search(list, a_key);
	printf("\n");

	test_insert(list, b_key, b_value);
	printf("\n");
	show_skip_list_level(list, 0);
	printf("\n");
	show_skip_list_level(list, 1);
	printf("\n");

	test_insert(list, a_key, a_value);
	printf("\n");
	test_insert(list, c_key, c_value);
	printf("\n");

	show_skip_list_level(list, 0);
	printf("\n");
	show_skip_list_level(list, 1);
	printf("\n");

	test_insert(list, d_key, d_value);
	printf("\n");
	test_insert(list, e_key, e_value);
	printf("\n");
	show_skip_list_level(list, 0);
	printf("\n");
	show_skip_list_level(list, 2);
	printf("\n");

	test_search(list, a_key);
	printf("\n");
	test_search(list, c_key);
	printf("\n");
	test_search(list, e_key);
	printf("\n");

	test_insert(list, g_key, g_value);
	printf("\n");
	test_insert(list, f_key, f_value);
	printf("\n");
	show_skip_list_level(list, 0);
	printf("\n");
	show_skip_list_level(list, 2);
	printf("\n");

	test_delete(list, c_key);
	printf("\n");
	show_skip_list_level(list, 0);
	printf("\n");
	show_skip_list_level(list, 1);
	printf("\n");
	show_skip_list_level(list, 2);
	printf("\n");
	show_skip_list_level(list, 3);
	printf("\n");

	skip_list_destroy(list);

	return 0;
}


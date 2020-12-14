#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <string.h>
#include "skip_list.h"

skip_list_node *skip_list_node_create(uint16_t level, key_type key, value_type value) {
	skip_list_node *node = (skip_list_node *)malloc(sizeof(skip_list_node));
	if (node == NULL) {
		return NULL;
	}
	memset(node, 0, sizeof(skip_list_node));

	node->next_nodes = (skip_list_node **)malloc(sizeof(skip_list_node *) * level);
	if (node->next_nodes == NULL) {
		free(node);
		return NULL;
	}
	memset(node->next_nodes, 0, sizeof(skip_list_node *) * level);

	node->level = level;
	node->key = key;
	node->value = value;

	return node;
}

void skip_list_node_free(skip_list_node *node) {
	if (node == NULL) {
		return;
	}

	if (node->next_nodes) {
		free(node->next_nodes);
	}

	free(node);
}

skip_list *skip_list_create(float prob) {
	if (!(0 < prob && prob < 1)) {
		return NULL;
	}

	skip_list *list = (skip_list *)malloc(sizeof(skip_list));
	if (list == NULL) {
		return NULL;
	}
	memset(list, 0, sizeof(skip_list));

	list->prob = prob;
	list->elem_num = 0;
	list->level = 1;

	list->header = skip_list_node_create(list->level, header_key, header_value);
	if (list->header == NULL) {
		free(list);
		return NULL;
	}

	return list;
}

bool skip_list_search(skip_list *list, key_type search_key, value_type *out) {
	if (list == NULL || list->header == NULL || out == NULL) {
		return false;
	}

	int i = 0;
	skip_list_node *x = list->header;
	for (i = list->level - 1; i >= 0; --i) {
		while (x->next_nodes[i] && key_lt(x->next_nodes[i]->key, search_key)) {
			x = x->next_nodes[i];
		}
	}

	x = x->next_nodes[0];
	if (x == NULL || !key_eq(x->key, search_key)) {
		return false;
	}

	*out = x->value;
	return true;
}

uint16_t get_max_len(uint32_t num) {
	uint16_t i = 1;
	while ((1 << i) < num) {
		++i;
	}

	return i;
}

uint16_t random_level(uint32_t num, float prob) {
	uint16_t v = 1;
	uint16_t max_len = get_max_len(num);

	srand((unsigned)time(NULL));
	while ((rand() * 1.0 / RAND_MAX) < prob && v < max_len) {
		++v;
	}
	printf("max_len: %u, random_level: %u\n", max_len, v);

	return v;
}

bool skip_list_update_header(skip_list *list) {
	if (list == NULL || list->header == NULL) {
		return false;
	}

	if (list->level == list->header->level) {
		return true;
	}

	skip_list_node **next_nodes = (skip_list_node **)realloc(list->header->next_nodes, list->level * sizeof(skip_list_node *));
	if (next_nodes == NULL) {
		return false;
	}

	list->header->level = list->level;
	list->header->next_nodes = next_nodes;

	return true;
}

bool skip_list_insert(skip_list *list, key_type search_key, value_type new_value) {
	skip_list_node **update = (skip_list_node **)malloc(list->level * sizeof(skip_list_node *));
	if (update == NULL) {
		return false;
	}
	memset(update, 0, sizeof(skip_list_node *) * list->level);

	int i = 0;
	skip_list_node *x = list->header;
	for (i = list->level - 1; i >= 0; --i) {
		while (x->next_nodes[i] && key_lt(x->next_nodes[i]->key, search_key)) {
			x = x->next_nodes[i];
		}
		update[i] = x;
	}

	x = x->next_nodes[0];
	if (x && key_eq(x->key, search_key)) {
		x->value = new_value;
		free(update);
		return true;
	}

	uint16_t v = random_level(list->elem_num + 1, list->prob);
	if (v > list->level) {
		update = (skip_list_node **)realloc(update, v * sizeof(skip_list_node *));
		if (update == NULL) {
			return false;
		}

		for (i = list->level; i < v; ++i) {
			update[i] = list->header;
		}
	}

	x = skip_list_node_create(v, search_key, new_value);
	if (x == NULL) {
		free(update);
		return false;
	}

	list->level = v;
	bool ret = skip_list_update_header(list);
	if (!ret) {
		free(update);
		return false;
	}

	for (i = 0; i < list->level; ++i) {
		x->next_nodes[i] = update[i]->next_nodes[i];
		update[i]->next_nodes[i] = x;
	}
	list->elem_num++;

	free(update);
	return true;
}

bool skip_list_delete(skip_list *list, key_type search_key) {
	skip_list_node **update = (skip_list_node **)malloc(list->level * sizeof(skip_list_node *));
	if (update == NULL) {
		return false;
	}
	memset(update, 0, sizeof(skip_list_node *) * list->level);

	int i = 0;
	skip_list_node *x = list->header;
	for (i = list->level - 1; i >= 0; --i) {
		while (x->next_nodes[i] && key_lt(x->next_nodes[i]->key, search_key)) {
			x = x->next_nodes[i];
		}
		update[i] = x;
	}

	x = x->next_nodes[0];
	if (x == NULL || !key_eq(x->key, search_key)) {
		free(update);
		return false;
	}

	for (i = 0; i < list->level; ++i) {
		if (update[i]->next_nodes[i] != x) {
			break;
		}
		update[i]->next_nodes[i] = x->next_nodes[i];
	}
	skip_list_node_free(x);

	while (list->level > 2 && list->header->next_nodes[list->level - 1] == NULL) {
		list->level--;
	}
	skip_list_update_header(list);
	list->elem_num--;

	free(update);
	return true;
}

void skip_list_destroy(skip_list *list) {
	if (list == NULL || list->header) {
		return;
	}

	skip_list_node *x = list->header;
	skip_list_node *y = NULL;

	while (x) {
		y = x->next_nodes[0];
		skip_list_node_free(x);
		x = y;
	}

	free(list);
}


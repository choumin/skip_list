#ifndef _SKIP_LIST_H_
#define _SKIP_LIST_H_

#include <stdint.h>
#include <stdbool.h>

#define key_type			uint32_t
#define value_type			void *

#define	header_key			0
#define header_value		NULL

#define key_eq(a, b)		(a == b)
#define key_lt(a, b)		(a < b)
#define key_gt(a, b)		(a > b)

typedef struct _skip_list_node{
	key_type key;
	value_type value;
	uint16_t level;
	struct _skip_list_node **next_nodes;
} skip_list_node;

typedef struct {
	uint32_t elem_num;
	uint16_t level;
	float	 prob;
	skip_list_node *header;
} skip_list;

skip_list_node *skip_list_node_create(uint16_t level, key_type key, value_type value);

skip_list *skip_list_create(float prob);

bool skip_list_search(skip_list *list, key_type search_key, value_type *out);

bool skip_list_insert(skip_list *list, key_type search_key, value_type new_value);

bool skip_list_delete(skip_list *list, key_type search_key);

void skip_list_destroy(skip_list *list);

#endif

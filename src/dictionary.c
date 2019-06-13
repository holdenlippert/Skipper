#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

Dict
emptydict()
{
	Dict empty = malloc(sizeof(struct dict));
	empty->key = NULL;
	empty->data = NULL;
	empty->next = NULL;
	return empty;
}

void
insert(Dict dict, char *key, void *data)
{
	if (dict->data == NULL) {
		dict->key = key;
		dict->data = data;
		dict->next = emptydict();
	}
	else
		insert(dict->next, key, data);
}

void *
get(Dict dict, char *key)
{
	if (dict->data == NULL)
		return NULL;
	if (strcmp(dict->key, key) == 0)
		return dict->data;
	return get(dict->next, key);
}

void
printdict(Dict dict)
{
	Dict original = dict;
	for (; dict->data != NULL; dict = dict->next) {
		printf("%s: %p\n", dict->key, get(original, dict->key));
	}
}

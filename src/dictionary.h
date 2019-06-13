struct dict {
	char *key;
	void *data;
	struct dict *next;
};
typedef struct dict * Dict;

Dict emptydict();
void insert(Dict dict, char *key, void *data);
void * get(Dict dict, char *key);
void printdict(Dict dict);

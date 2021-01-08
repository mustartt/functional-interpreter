#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include "dynamic.h"

/* Dictionary Entry */
struct entry_t {
	struct entry_t* next;
	//bool build_in;
	char* key;
	struct DynamicVar *expression;
};

unsigned hash(char* s);
struct entry_t** create_hashtab();
struct DynamicVar* hashtab_lookup(struct entry_t** hashtab, char* s);
struct entry_t* hashtab_install(struct entry_t** hashtab, char* key, struct DynamicVar* value);
void free_hashtable(struct entry_t** hashtab);

void debug_hashtab(struct entry_t** hashtab);

#endif
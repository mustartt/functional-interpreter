#include <stdio.h>
#include <string.h>

#include "dynamic.h"
#include "dictionary.h"
#include "scope.h"


struct Scope* create_scope(char** indentifier, struct DynamicVar** values, int length, struct Scope *parent) {
	struct Scope* scope = (struct Scope*)malloc(sizeof(struct Scope));

	scope->parent = parent;
	scope->self = create_hashtab();
	
	int i;
	for (i = 0; i < length; i++) {
		// duplicate string and value
		char* key = strdup(indentifier[i]);
		struct DynamicVar* var = vardup(values[i]);
		hashtab_install(scope->self, key, var);
	}

	return scope;
}


/* Check if identifier is in self and then in parent scope */
struct entry_t* scope_lookup(struct Scope *scope, char* identifier) {
	
	struct entry_t* entry = hashtab_lookup(scope->self, identifier);

	if (entry != NULL) {
		// found in self -> return the expression
		return entry;
	}
	else if (scope->parent == NULL) {
		return NULL; // Not found in global scope
	}
	else {
		// look up in higher scope
		return scope_lookup(scope->parent, identifier);
	}
}


void free_scope(struct Scope *scope) {
	if (scope->parent != NULL)
		free_scope(scope->parent);
	hashtable_free(scope->self);
}
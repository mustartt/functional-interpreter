#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic.h"
#include "dictionary.h"
#include "scope.h"


struct Scope* create_scope(char** indentifier, struct DynamicVar** values, int length, struct Scope *parent) {
	
	struct Scope* scope = (struct Scope*) malloc(sizeof(struct Scope));

	if (scope != NULL) {
		scope->parent = parent;
		scope->self = create_hashtab();
	
		int i;
		for (i = 0; i < length; i++) {
			// hashtab_install duplicates k,v internally
			// DEBUG: printf("Installing key=%s val=%p\n", indentifier[i], values[i]);
			hashtab_install(scope->self, indentifier[i], values[i]);
		}
	}

	return scope;
}


/* Check if identifier is in self and then in parent scope */
struct DynamicVar* scope_lookup(struct Scope *scope, char* identifier) {
	
	struct DynamicVar* entry = hashtab_lookup(scope->self, identifier);

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

/* Only needs to free self */
void free_scope(struct Scope *scope) {
	//if (scope->parent != NULL)
		//free_scope(scope->parent);
	free_hashtable(scope->self);
	free(scope);
}
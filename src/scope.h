#ifndef SCOPE_H
#define SCOPE_H

#include "dictionary.h"

struct Scope {
	struct Scope* parent;
	struct entry_t** self;
};

struct Scope* create_scope(char** indentifier, struct DynamicVar** values, int length, struct Scope* parent);
struct DynamicVar* scope_lookup(struct Scope* scope, char* identifier);
void free_scope(struct Scope* scope);

#endif
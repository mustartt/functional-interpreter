#ifndef LEXER_H
#define LEXER_H

struct TokenList {
	int length;
	char** tokens;
};

char** tokenize(char* str, int* length);
char* read_file(char* filename);
struct DynamicVar* atomize(char* statement);
struct DynamicVar* action_tree(struct TokenList* token_list);
struct DynamicVar* parse(char** tokens, int length);

#endif
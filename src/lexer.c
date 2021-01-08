#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "dynamic.h"
#include "lexer.h"

struct StringStruct {
	int length;
	int total_size;
	char* content;
};

char* read_file(char* filename) {
	FILE* file;
	file = fopen(filename, "r");
	int c;

	if (file) {
		/* does not initialize null terminator */
		struct StringStruct str;
		str.length = 0;
		str.total_size = 1;
		str.content = (char*)malloc(sizeof(char));
		str.content[0] = '\0'; 

		do {
			c = getc(file);
			
			if (str.length == str.total_size) { /* expand */
				str.total_size *= 2;
				char* temp = (char*)realloc(str.content, str.total_size * sizeof(char));
				if (temp == NULL) {
					free(str.content);
					fclose(file);
					return NULL;
				}
				str.content = temp;
			}

			str.content[str.length] = (char)c;
			str.length += 1;
		} while (c != EOF);

		fclose(file); /* closes the file */

		/* sets the null terminator */
		str.content[str.length - 1] = '\0'; 

		return str.content;
	}

	return NULL;
}


char* pad_bracket(char* str, char bracket, int index) {
	int len = strlen(str);
	char* temp = realloc(str, (len + 2 + 1) * sizeof(char)); /* inclues null terminator */
	
	if (temp == NULL) {
		free(str);
		return NULL;
	}

	str = temp;

	int i;
	for (i = len + 2; i >= index; i--) {
		str[i] = str[i - 2];
	}

	/* set spaces */
	str[index] = ' ';
	str[index + 1] = bracket;
	str[index + 2] = ' ';

	return str;
}

char** tokenize(char* program, int* length) {
	char* str = strdup(program); /* free this at the end */

	// string pre-processing
	int len = strlen(str);
	int i, j;

	/* remove comments # -> \n */
	int start = strcspn(str, "#");
	int end   = start + strcspn(str + start * sizeof(char), "\n");

	while (start < len) {
		for (i = start; i <= end; i++) {
			str[i] = ' ';
		}
		start = strcspn(str, "#");
		end   = start + strcspn(str + start * sizeof(char), "\n");
	}

	/* replace tab and newline with spaces */
	for (i = 0; i < len; i++) {
		if (str[i] == '\t' || str[i] == '\n' || str[i] == ';') {
			str[i] = ' ';
		}
	}

	char brackets[] = { '(', ')', '[', ']', '{', '}' };
	/* insert spaces between brackets */
	for (i = 0; i < len; i++) {
		bool is_bracket = false;
		char brack = '@';
		for (j = 0; j < 6; j++) {
			if (str[i] == brackets[j]) {
				is_bracket = true;
				brack = brackets[j];
			}
		}

		if (is_bracket == true) {
			//printf("%c\n", brack);
			str = pad_bracket(str, brack, i);
			len += 2;
			i += 1;
		}
	}

	/* initalize tokens */
	char** tokens = (char**)malloc(sizeof(char*));
	if (tokens == NULL) {
		free(str);
		return NULL;
	}

	/* tokenize */
	int counter = 1;
	char* target = " ";
	
	char* token = strtok(str, target);
	tokens[0] = strdup(token);

	while (token != NULL) {
		token = strtok(NULL, target);

		/* probably should fix this */
		char** temp = realloc(tokens, (counter + 1) * sizeof(char*));
		if (temp == NULL) {
			free(str);
			free(tokens);
			return NULL;
		}
		tokens = temp;
		tokens[counter] = strdup(token);
		counter++;
	}

	*length = counter - 1;
	free(str);
	return tokens;
}




/* lexer parsing functions */

bool is_opening(const char* str) {
	return strcmp(str, "(") == 0 || strcmp(str, "[") == 0 || strcmp(str, "{") == 0;
}

bool is_closing(const char* str) {
	return strcmp(str, ")") == 0 || strcmp(str, "]") == 0 || strcmp(str, "}") == 0;
}


char* pop(struct TokenList* token_list) {
	char* token = token_list->tokens[0];
	
	/* shifts the tokens left by one */
	memmove(token_list->tokens, token_list->tokens + 1, (token_list->length - 1) * sizeof(char *));
	token_list->length -= 1;

	return token;
}

struct DynamicVar* parse(char** tokens, int length) {
	/* malloc the single struct */
	struct TokenList* token_list = (struct TokenList*)malloc(sizeof(struct TokenList));
	int i;

	if (token_list == NULL) {
		/* free the tokens */
		int i;
		for (i = 0; i < length; i++) {
			free(tokens[i]);
		}
		free(tokens);
		return NULL;
	}

	/* makes a copy of tokens to free later */
	char** old_ptrs = (char**)malloc(length * sizeof(char*));
	if (old_ptrs == NULL) {
		/* free the tokens */
		int i;
		for (i = 0; i < length; i++) {
			free(tokens[i]);
		}
		free(tokens);
		return NULL;
	}
	
	memcpy(old_ptrs, tokens, length * sizeof(char*)); /* copy the old pointers */

	token_list->length = length;
	token_list->tokens = tokens; /* safe to pass pointer since a copy is made */

	struct DynamicVar* result = action_tree(token_list); /* calls action tree */
	
	/*print_var_debug(result);
	printf("\n");*/

	/* free the tokens */
	for (i = 0; i < length; i++) {
		free(old_ptrs[i]);
	}

	free(tokens); /* bug here */
	free(old_ptrs);
	/* free token_list */
	free(token_list);

	return result;
}

struct DynamicVar* action_tree(struct TokenList* token_list) {

	if (token_list->length == 0) {
		printf("Expected an expression, but there were no more tokens.\n");
		return NULL;
	}

	char* token = pop(token_list);

	if (is_opening(token)) { /* lambda expressions */

		struct DynamicList* result = create_list();

		while (!is_closing(token_list->tokens[0])) {
			struct DynamicVar* subtree = action_tree(token_list);
			insert(result, subtree);
		}
	
		pop(token_list);

		/* check if lambda has parameters attached */
		/* (lambda({x} -> x))(param) */

		if (token_list->length != 0 && is_opening(token_list->tokens[0])) {
			pop(token_list);

			while (!is_closing(token_list->tokens[0])) {
				struct DynamicVar* subtree = action_tree(token_list);
				insert(result, subtree);
			}

			pop(token_list);
		}

		/* creates the nested expression */
		struct DynamicVar* var = create_var();
		var->var_type = ListType;
		var->list_struct_ptr = result;
		return var;
	}
	else if (token_list->length != 0 && is_opening(token_list->tokens[0])) {
		/* parse function call */
		struct DynamicList* result = create_list();
		insert(result, atomize(token)); /* token is guranteed to be str */
		pop(token_list);

		while (!is_closing(token_list->tokens[0])) {
			struct DynamicVar* subtree = action_tree(token_list);
			insert(result, subtree);
		}

		pop(token_list);

		/* creates the nested expression */
		struct DynamicVar* var = create_var();
		var->var_type = ListType;
		var->list_struct_ptr = result;
		return var;
	}
	else {
		return atomize(token);
	}
}



/* duplicates a new value */
struct DynamicVar* atomize(char* statement) {
	/* If statement contains "." then it is a floating value */
	unsigned int i;
	bool is_float = false;
	bool is_numeric = true;

	for (i = 0; i < strlen(statement); i++) {
		if (statement[i] == '.') {
			is_float = true;
			is_numeric = true;
			break;
		}
		else if (!isdigit(statement[i])) {
			is_numeric = false;
			is_float = false;
			break;
		}
	}

	if (is_numeric) {
		if (is_float) {
			float float_val;
			int ret = sscanf(statement, "%f", &float_val);

			struct DynamicVar* var = create_var();
			var->var_type = FloatType;
			var->float_value = float_val;

			return var;
		}
		else {
			int int_val;
			int ret = sscanf(statement, "%d", &int_val);

			struct DynamicVar* var = create_var();
			var->var_type = IntType;
			var->int_value = int_val;

			return var;
		}
	}
	else {
		// parse struct
		struct DynamicVar* var = create_var();
		var->var_type = StringType;
		var->string_ptr = strdup(statement);

		return var;
	}

	return NULL;
}


#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
#include "dynamic.h"
#include "dictionary.h"
#include "scope.h"
#include "standard_library.h"
#include "lexer.h"

// Expression Tree
// λx. x -> ['λ', ['x'], 'x']
// ['+', 2, 3]

// Expression Tree contains 
/*
 * IntType
 * FloatType
 * StringType -> str is represented as "expression"
 * ListType -> Can be nested with any types of elements
 * StructType -> Key-value pairs dict[str, Any]
 */

char* _IF = "if";
char* _LAMBDA = "lambda";
char* _ARROW = "->";


struct DynamicVar* evaluate(struct DynamicVar* exp, struct Scope* scope);

int compare_first(const char* str1, struct DynamicVar* exp) {
	struct DynamicVar* first = get(exp->list_struct_ptr, 0);
	int result = strcmp(str1, first->string_ptr);

	return result;
}

struct DynamicVar* eval_af(struct AnonymousFunction* func, struct DynamicList* args) {
	// BUG: create_scope takes char** identifier
	char** idents = (char**)malloc(func->params->length * sizeof(char*));
	int i;
	for (i = 0; i < func->params->length; i++) {
		idents[i] = strdup(get(func->params, i)->string_ptr);
	}

	struct Scope* call_stack = create_scope(idents, args->list, args->length, func->scope);

	/* free the parameters */
	for (i = 0; i < func->params->length; i++) {
		free(idents[i]);
	}
	free(idents);
	
	if (call_stack == NULL)
		return NULL;

	struct DynamicVar* result = evaluate(func->body, call_stack);
	// free_scope(call_stack); /* frees the call_stack */
	return result;
}

static struct DynamicList* intermediate_expressions;


struct DynamicVar* evaluate(struct DynamicVar* exp, struct Scope* scope) {

	/* 
	 * Check for expression tree first 
	 * ['function', p_1, ..., p_n]
	 * ['lambda', ['x'], 'x']
	 * [['lambda', ['x'], 'x'], 5]
	 */

	/* Lambda Expressions Formats */
	/* ['lambda', ['p_1', ... 'p_n'], '->', exp] */
	
	if (exp->var_type == StringType) {
		
		struct DynamicVar* result = scope_lookup(scope, exp->string_ptr);
	
		if (result == NULL) {
			printf("The identifier %s is not found.", exp->string_ptr);
			exit(-1);
		}
		else {
			return result;
		}
	}
	else if (exp->var_type != ListType) {
		return exp;
	}
	else if (compare_first(_IF, exp) == 0) {
		
	}
	else if (compare_first(_LAMBDA, exp) == 0) {
		
		struct DynamicVar* params = get(exp->list_struct_ptr, 1);
		struct DynamicVar* lambda_expr = get(exp->list_struct_ptr, 3);

		/* need to duplicate the variables for evaluation */
		struct AnonymousFunction* func = create_af(params->list_struct_ptr, lambda_expr, scope);

		struct DynamicVar* var = create_var();
		var->var_type = LambdaType;
		var->anon_func_struct = func;

		if (func == NULL || var == NULL)
			return NULL;

		return var;
	}
	else {
		
		struct DynamicVar* prefix = evaluate(get(exp->list_struct_ptr, 0), scope);

		struct DynamicList* args = create_list();
		int i;
		for (i = 1; i < exp->list_struct_ptr->length; i++) {
			struct DynamicVar* positional = get(exp->list_struct_ptr, i);
			struct DynamicVar* param_result = evaluate(positional, scope);

			if (param_result == NULL)
				return NULL;

			insert(args, param_result);
		}

		// check for built-in or lambda expression
		if (prefix->var_type == FunctionType) { 
			struct DynamicVar* result = (*prefix->func_ptr)(args);
			
			/* free the list it self but not the items */
			free(args->list);
			free(args);

			/* add results to intermediate expressions to be freed later */
			/* only built-in functions can produce new malloc'd results. */
			insert(intermediate_expressions, result);

			return result;
		}
		else {
			// prefix must be lambda expression
			struct DynamicVar* result = eval_af(prefix->anon_func_struct, args);

			/* free the list it self but not the items */
			free(args->list);
			free(args);

			return result;
		}
	}

	return NULL;

}





/* Playing around with testing the interpreter */
void test_advanced(void);

int main(int argc, char** argv) {

	//_CrtSetBreakAlloc(142);

	
	test_advanced();
	
	// detects memory leak
	_CrtDumpMemoryLeaks();
	return 0;
}


void test_advanced() {
	struct Scope* global = global_scope();     /* malloc  */
	intermediate_expressions = create_list();

	printf("Global Scope Debugging Info: \n");
	debug_hashtab(global->self);

	char* str = read_file("test.lang");

	printf("============================\n");
	printf("%s\n", str);
	printf("============================\n");

	int len, i;
	char** tokens = tokenize(str, &len);

	if (tokens != NULL) {
		printf("[");
		for (i = 0; i < len; i++) {
			printf("%s", tokens[i]);
			if (i != len - 1)
				printf(", ");
		}
		printf("]\n");

		// evaluate action_tree()
		struct DynamicVar* exp_tree = parse(tokens, len);
		printf("Parsed Result = ");
		print_var_debug(exp_tree);
		printf("\n");

		// evaluates
		struct DynamicVar* result = evaluate(exp_tree, global);
		printf(" === Evaluated Result === \n");
		print_var_debug(result);
		printf("\n");

		// free intermediate expressions
		free_list(intermediate_expressions);

		free_var(exp_tree);
	}

	free(str);
	free_scope(global);
}

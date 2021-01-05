#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dynamic.h"
#include "dictionary.h"
#include "scope.h"

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


/* Creates a new DynamicVar */
struct DynamicVar* add(struct DynamicVar* var1, struct DynamicVar* var2) {
	struct DynamicVar* var = create_var();

	if (var1->var_type == FloatType || var2->var_type == FloatType) {
		float result = var1->float_value + var2->float_value;
		var->var_type = FloatType;
		var->float_value = result;
	}
	else {
		int result = var1->float_value + var2->float_value;
		var->var_type = IntType;
		var->float_value = result;
	}

	return var;
}

struct Scope* get_global_scope() {
	// create empty scope 
	struct Scope* global = create_scope(NULL, NULL, 0, NULL);

	char* op = "+";
	struct DynamicVar* function = create_var();
	function->var_type = FunctionType;
	function->func_ptr = &add;
	struct entry_t* entry = hashtab_install(global, op, function);


	
	free_var(function);
	return global;
}



struct DynamicVar* evaluate(struct DynamicVar* exp, struct Scope* scope) {

	/* 
	 * Check for expression tree first 
	 * ['function', p_1, ..., p_n]
	 * ['lambda', ['x'], 'x']
	 * [['lambda', ['x'], 'x'], 5]
	 */

	if (exp->var_type == StringType) {
		struct entry_t* entry = scope_lookup(scope, exp->string_ptr);

		if (entry == NULL) {
			// raise error
			printf("The identifier %s is not found.\n", exp->string_ptr);
		}
		else {
			return entry->expression;
		}
	}
	

	return NULL;

}






int main(int argc, char** argv) {

	char* op = "+";

	// ['+', 123, 456] -> 579
	struct DynamicVar* expression = create_var();
	expression->var_type = ListType;

	struct DynamicVar* func = create_str_var(op);

	struct DynamicVar* num1 = create_var();
	num1->var_type = IntType;
	num1->int_value = 123;
	struct DynamicVar* num2 = create_var();
	num2->var_type = IntType;
	num2->int_value = 456;

	struct DynamicList* list = create_list();
	insert(list, func);
	insert(list, num1);
	insert(list, num2);
	expression->list_struct_ptr = list;


	// TESTING

	struct Scope* global_scope = get_global_scope();

	struct entry_t* res = scope_lookup(global_scope, strdup(op));



	//free_scope(global_scope);
	free_var(expression);

	return 0;
}
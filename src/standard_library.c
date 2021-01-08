#include <stdio.h>
#include <stdlib.h>

#include "dynamic.h"
#include "scope.h"
#include "standard_library.h"

/* All built-in function returns a new malloc DynamicVar */

/* FUNCTION: add */
struct DynamicVar* add(struct DynamicList* args) {
	struct DynamicVar* var = create_var();

	struct DynamicVar* var1 = get(args, 0);
	struct DynamicVar* var2 = get(args, 1);

	if (var1->var_type == FloatType || var2->var_type == FloatType) {
		float v1 = (float)(var1->var_type == FloatType) ? var1->float_value : var1->int_value;
		float v2 = (float)(var2->var_type == FloatType) ? var2->float_value : var2->int_value;

		var->var_type = FloatType;
		var->float_value = v1 + v2;
	}
	else {
		int result = var1->int_value + var2->int_value;
		var->var_type = IntType;
		var->int_value = result;
	}

	return var;
}



struct Scope* global_scope() {
	struct Scope* global = create_scope(NULL, NULL, 0, NULL);

	if (global != NULL) {

		// constants
		char* pi_sym = "pi";
		struct DynamicVar* pi = create_var();
		pi->var_type = FloatType;
		pi->float_value = (float) 3.14159;

		hashtab_install(global->self, pi_sym, pi);
		free_var(pi);

		// add
		char* op = "+";
		struct DynamicVar* add_func = create_var();
		add_func->var_type = FunctionType;
		add_func->func_ptr = &add;
		
		hashtab_install(global->self, op, add_func);
		free_var(add_func);



	}

	return global;
}


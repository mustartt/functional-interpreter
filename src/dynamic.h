#ifndef DYNAMIC
#define DYNAMIC

enum Type { IntType, FloatType, StringType, ListType, StructType, FunctionType, LambdaType };

struct DynamicVar {
	enum Type var_type;
	union {
		int int_value;
		float float_value;
		char* string_ptr;
		struct DynamicList* list_struct_ptr;
		// StructType
		struct DynamicVar* (*func_ptr)(struct DynamicList*);
		struct AnonymousFunction* anon_func_struct;
	};
};

struct AnonymousFunction {
	struct DynamicList* params;
	struct DynamicVar* body;
	struct Scope* scope;
};

struct DynamicList {
	int length;
	int total_size;
	struct DynamicVar** list;
};

/* AnonymousFunction Functions */
struct AnonymousFunction* create_af(struct DynamicList* params, struct DynamicVar* body, struct Scope* scope);
struct DynamicVar* eval_af(struct AnonymousFunction* func, struct DynamicList* args);
void free_af(struct AnonymousFunction* func);

/* DynamicVar Functions */
struct DynamicVar* create_var(void);
void free_var(struct DynamicVar* var);
struct DynamicVar* create_str_var(char* str);
struct DynamicVar* vardup(struct DynamicVar* var);

/* DynamicList Functions */
struct DynamicList* create_list(void);
void free_list(struct DynamicList* list);
struct DynamicVar* get(struct DynamicList* list, int index);
struct DynamicVar* insert(struct DynamicList* list, struct DynamicVar* var);
struct DynamicVar* insert_at(struct DynamicList* list, struct DynamicVar* var, int index);
struct DynamicList* listdup(struct DynamicList* list);

/* Untility Functions */

struct DynamicList* create_int_list(int* arr, int arr_size);

void print_list(struct DynamicList* list);
void print_var(struct DynamicVar* var);

void print_list_debug(struct DynamicList* list);
void print_var_debug(struct DynamicVar* var);

#endif
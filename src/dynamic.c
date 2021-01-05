#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic.h"



/*
 * Function: *create_var
 * ---------------------
 * creates the DynamicVar and returns the pointer
 *
 * returns: the pointer pointing to the DynamicVar
 */
struct DynamicVar *create_var() {
  return malloc(sizeof(struct DynamicVar));
}


/*
 * Function: free_var
 * ------------------
 * frees the DynamicVar from memory and its nested children
 *
 * *var: the pointer to the DynamicVar in heap 
 */
 void free_var(struct DynamicVar *var) {
   if (var->var_type == ListType) {
     free_list(var->list_struct_ptr);
   } else if (var->var_type == StringType) {
     free(var->string_ptr);
   }
   free(var);
 }


struct DynamicVar *create_str_var(char *str) {
  struct DynamicVar *var = create_var();
  var->var_type = StringType;

  // copy to heap
  var->string_ptr = malloc((strlen(str) + 1) * sizeof(char));
  strcpy(var->string_ptr, str);
  
  return var;
}


struct DynamicVar* vardup(struct DynamicVar* var) {
    struct DynamicVar* new_var = create_var();
    
    if (var->var_type == ListType) {
        new_var->var_type = ListType;
        struct DynamicList* new_list = listdup(var->list_struct_ptr);
        new_var->list_struct_ptr = new_list;
    }
    else if (var->var_type == StringType) {
        new_var->var_type = StringType;
        new_var->string_ptr = strdup(var->string_ptr);
    }
    else {
        memcpy(new_var, var, sizeof(struct DynamicVar));
    }

    return new_var;
}



/* ======= Dynamic List ======= */

/*
 * Function: *create_list
 * ----------------------
 * creates the DynamicList struct with inital size of 4 and malloc the 
 *    list of size 4
 *
 * returns: the pointer to the DynamicList struct with the internal list 
 *          already allocated
 */
struct DynamicList *create_list() {
  // malloc the list it self
  struct DynamicList *new_list = malloc(sizeof(struct DynamicList));

  // set the internal state
  new_list->length = 0;
  new_list->total_size = 4;
  
  // malloc the internal list
  new_list->list = malloc(new_list->total_size * sizeof(struct DynamicVar *));
  return new_list;
}



/*
 * Function: free_list
 * -------------------
 * free all elements in the DynamicList. If the element is a StringType, 
 *    free the String. If the element is a ListType, free the DynamicList.
 *    Free the list of pointer and the struct representation it self. 
 * 
 * *list: pointer of the list struct
 *
 * returns: void
 */
void free_list(struct DynamicList *list) {
  // free the list elements
  int i;
  for (i = 0; i < list->length; i++) {
    struct DynamicVar *var = list->list[i];
    free_var(var);
  }
  // free the internal list in list
  free(list->list);
  free(list);
}



/* 
 * Function: *get
 * --------------
 * gets the DynamicVar at index 
 *
 * *list: the list representation of DynamicList
 * index: the index of the item to get
 *
 * returns: the pointer to the DynamicVar at index 
 */
struct DynamicVar *get(struct DynamicList *list, int index) {
  return list->list[index];
}



/*
 * Function: insert
 * ----------------
 * inserts the var at the end of the list. Expands the array by size 2 if it exceed
 *    current size. 
 *
 * *list: the list to insert on
 * *var: the DynamicVar to be inserted
 * 
 * returns: void
 */
void insert(struct DynamicList *list, struct DynamicVar *var) {
  if (list->length == list->total_size) {
    // expand array by 2
    list->total_size *= 2;
    list->list = realloc(list->list, list->total_size * sizeof(struct DynamicVar *));
  }
  // insert as usual
  list->list[list->length] = var;
  list->length += 1;
}



/*
 * Function: insert_at
 * -------------------
 * inserts the var at index into list. Expands the array by size 2 if it exceed
 *    current size. 
 *
 * *list: the list to insert on
 * *var: the DynamicVar to be inserted
 * index: the index to insert at 
 *
 * returns: void
 */
void insert_at(struct DynamicList *list, struct DynamicVar *var, int index) {
  if (list->length == list->total_size) {
    // expand array by 2
    list->total_size *= 2;
    struct DynamicVar* temp = (struct DynamicVar*) 
        realloc(list->list, list->total_size * sizeof(struct DynamicVar *));

    if (temp != NULL) {
        list->list = temp;
        // expand list length
        list->length += 1;
        // make space: copy pointers after index to the right by one
        int i;
        for (i = list->length - 1; i > index; i--) {
            list->list[i] = list->list[i - 1];
        }
        // insertion
        list->list[index] = var;
    }
  }
}



struct DynamicList* listdup(struct DynamicList* list) {
    struct DynamicList* new_list = create_list();

    int i;
    for (i = 0; i < list->length; i++) {
        insert(new_list, vardup(get(list, i)));
    }

    return new_list;
}




/*
 * Function: partition
 * -------------------
 * create a new DynamicList with the content at starting index to end index
 * 
 * start: starting index
 * end:   ending index
 * 
 * returns: a new copy of DynamicList with content between starting and end index non-inclusive
 */
struct DynamicList* partition(struct DynamicList* list, int start, int end) {
    struct DynanmicList* new_list = create_list();



    return new_list;
}



/* Untility Functions */

struct DynamicList *create_int_list(int *arr, int arr_size) {
  struct DynamicList *list = create_list();
  int i;
  
  for (i = 0; i < arr_size; i++) {
    struct DynamicVar *var = create_var();
    var->var_type = IntType;
    var->int_value = arr[i];
    
    insert(list, var);
  }
  
  return list;
}

void print_var(struct DynamicVar *var) {
  switch (var->var_type) {
    case IntType:
      printf("%d", var->int_value);
      break;
    case FloatType:
      printf("%f", var->float_value);
      break;
    case StringType:
      printf("%s", var->string_ptr);
      break;
    case ListType:
      print_list(var->list_struct_ptr);
      break;
    // StructType
    default:
      printf("Unknown Var.");
  }
}



void print_list(struct DynamicList *list) {
  printf("[");
  int i;
  for (i = 0; i < list->length; i++) {
    struct DynamicVar *var = list->list[i];
    print_var(var);
    printf(i == list->length - 1 ? "]" : ", ");
  }
}



/* DEBUGGING FUNCTIONS */

void print_var_debug(struct DynamicVar *var) {
  switch (var->var_type) {
    case IntType:
      printf("<Int %d>", var->int_value);
      break;
    case FloatType:
      printf("<Float %f>", var->float_value);
      break;
    case StringType:
      printf("<String %s>", var->string_ptr);
      break;
    case ListType:
      printf("<List ");
      print_list(var->list_struct_ptr);
      printf(">");
      break;
    // StructType
    default:
      printf("Unknown Type: %d.", var->var_type);
  }
}

void print_list_debug(struct DynamicList *list) {
  printf("[");
  int i;
  for (i = 0; i < list->length; i++) {
    struct DynamicVar *var = list->list[i];
    print_var_debug(var);
    printf(i == list->length - 1 ? "]" : ", ");
  }
}

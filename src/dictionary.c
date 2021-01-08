#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dynamic.h"
#include "dictionary.h"

#define HASHSIZE 101

/* hashing function */
unsigned hash(char* s) {
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/*
 * hashtab is a pointer table pointing to struct entry_t
 * struct entry_t** hashtab
 */

/* Creates the hasttab itself */
struct entry_t** create_hashtab() {
	return (struct entry_t**) calloc(HASHSIZE, sizeof(struct entry_t*));
}


/* Look up s in hashtab */
/* NOTE: Make sure to duplicate before using */
struct DynamicVar* hashtab_lookup(struct entry_t** hashtab, char* s) {
	struct entry_t* head;
	unsigned hashval = hash(s);
	for (head = hashtab[hashval]; head != NULL; head = head->next)
		if (strcmp(s, head->key) == 0)
			return head->expression;
	// if not found
	return NULL;
}


/* Look up s in hashtab */
/* NOTE: Make sure to duplicate before using */
struct entry_t* lookup(struct entry_t** hashtab, char* s) {
	struct entry_t* head;
	unsigned hashval = hash(s);
	for (head = hashtab[hashval]; head != NULL; head = head->next)
		if (strcmp(s, head->key) == 0)
			return head;
	// if not found
	return NULL;
}




/* 
 * put key and value in hastab 
 * NOTE: rememeber to free the DynamicVar, its not freed in here
 */
struct entry_t* hashtab_install(struct entry_t** hashtab, char* key, struct DynamicVar* value) {
	
	// DEBUG: printf("Installing key=%s val=%p\n", key, value);
	
	struct entry_t* head = lookup(hashtab, key);
	unsigned hashval;

	/* Not Found */
	if (head == NULL) {
		head = (struct entry_t*) malloc(sizeof(struct entry_t));
		
		if (head == NULL || (head->key = strdup(key)) == NULL)
			return NULL;
		
		hashval = hash(key);
		head->next = hashtab[hashval];
		hashtab[hashval] = head;
	} else {
		// free previous definion when its a DynamicList
		free_var(head->expression);
	}

	// duplicate DynamicVar
	if ((head->expression = vardup(value)) == NULL)
		return NULL;

	// DEBUG: printf("The new pointer is val=%p\n", head->expression);

	return head;
}

/* Free Hashtable */
void free_hashtable(struct entry_t** hashtab) {
	int i;
	for (i = 0; i < HASHSIZE; i++) {
		if (hashtab[i] != NULL) {

			//DEBUG: printf("Freeing %p\n", hashtab[i]);

			struct entry_t* head = hashtab[i];
			struct entry_t* temp; 

			while (head != NULL) {
				temp = head;
				head = head->next;

				// free the internal states
				free(temp->key);
				free_var(temp->expression);
				free(temp);
			}
		}
	}

	free(hashtab);
}

/* Prints the debug information */
void debug_hashtab(struct entry_t** hashtab) {
	printf("=================================\n");
	for (int i = 0; i < HASHSIZE; i++) {
		if (hashtab[i] != NULL) {
			printf("hash=%d key=\"%s\" exp=%p next=%p\n",
				i,
				hashtab[i]->key,
				hashtab[i]->expression,
				hashtab[i]->next);
		}
	}
	printf("=================================\n");
}

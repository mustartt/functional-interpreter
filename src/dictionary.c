#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dynamic.h"
#include "dictionary.h"


/*
 * Function: hash
 * --------------
 * form has value for string s
 */
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
	struct entry_t* hashtab = (struct entry_t*) calloc(HASHSIZE, sizeof(struct entry_t*));
	return hashtab;
}


/* Look up s in hashtab */
struct entry_t* hashtab_lookup(struct entry_t** hashtab, char* s) {
	// entry head
	struct entry_t* head;
	for (head = hashtab[hash(s)]; head != NULL; head = head->next)
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
	struct entry_t* head;
	unsigned hashval;

	/* Not Found */
	if ((head = hashtab_lookup(hashtab, key)) == NULL) {
		head = (struct entry_t*)malloc(sizeof(struct entry_t));
		if (head == NULL || (head->key = strdup(key)) == NULL)
			return NULL;
		hashval = hash(key);
		head->next = hashtab[hashval];
		hashtab[hashval] = head;
	} else {
		// free previous definion when its a DynamicList
		free_var(head->expression);

		// duplicate DynamicVar
		head->expression = vardup(value);
	}

	// build-in operations dont have expressions
	if (value != NULL) {
		// duplicate DynamicVar
		head->expression = vardup(value);
	}

	return head;
}

/* Free Hashtable */
void hashtable_free(struct entry_t** hashtab) {
	int i;
	for (i = 0; i < HASHSIZE; i++) {
		if (hashtab[i] != NULL) {
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
}


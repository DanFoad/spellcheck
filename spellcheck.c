/**
 * Spellchecking Programme
 * DevChannel Biweekly Challenge #1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Error
 * Structure to hold Linked List nodes for Error type
 */
typedef struct node {
	int correction_no;
	char *word, **correction;
	struct node *next;
} Error;

/** createErrorNode
 * Construct Error type struct from input data
 * @param char* word			The word that was not found in the dictionary
 * @param char** correction		Array of possible corrections
 * @param int flag				Whether node has corrections or not or is root
 * @param int count 			The number of possible corrections found
 * @return Error*				The Error type node containing the input data
 */
Error* createErrorNode(char* word, char** correction, int flag, int count) {

	int i = 0;

	Error *node = (Error*)malloc(sizeof(Error));
	
	/* If flag is set to two, node is root */
	if (flag == 2) {
		node->word = (char*)malloc(5 * sizeof(char));
		strcpy(node->word, "root");

		node->correction_no = 1;
		node->correction = (char**)malloc(sizeof (char*));
		node->correction[i] = (char*)malloc(5 * sizeof (char));
		strcpy(node->correction[0], "root");
	} else {
		node->word = (char*)malloc((strlen(word) + 1) * sizeof(char));
		strcpy(node->word, word);
	}

	/* If flag is set to zero, no corrections were found */
	if (flag == 0) {
		node->correction_no = 1;
		node->correction = (char**)malloc(sizeof (char*));
		node->correction[i] = (char*)malloc(13 * sizeof (char));
		strcpy(node->correction[0], "NOCORRECTION");
	}

	/* If flag is set to one, all possible corrections are copied to the element in the list, along with the number of errors */
	if(flag == 1) {
	node->correction_no = count;
	node->correction = (char**)malloc((count)* sizeof(char*));
		for (i = 0; i < count; i++) {
			node->correction[i] = (char*)malloc((strlen(correction[i]) + 1) * sizeof(char));
			strcpy(node->correction[i], correction[i]);
		}
	}

	/* Initialise next node as null pointer */
	node->next = 0;
	return node;
}

/** deallocateLinkedList
 * Free all the nodes of the linked list, including their data
 * @param Error* root The root node of the Linked List
 */
void deallocateLinkedList(Error *root){

	int i = 0;
	struct node * temp;

	while (root != NULL) {

		/* Sets the current node pointer to a temporary node and then frees the allocated memory through the temporary pointer */
		temp = root;
		root = root->next;

		/* Frees all allocated data in the temporary element, then frees the element */
		free(temp->word);

		for (i = 0; i < temp->correction_no; i++) {
			free(temp->correction[i]);
		}

		free(temp->correction);
		free(temp);
	}
}

/** deallocateDict
 * Free all the elements of the dictionary, and then the dict itself
 * @param char** dict The dictionary to de-allocate
 */
void deallocateDict(char** dict) {
    int i = 0;

	/* Frees every word in the dictionary, then frees the dictionary array */
	for (i = 0; i < 1000; i++)
		free(dict[i]);

	free(dict);
}

/** deallocateCorrections
 * Free all the elements of a corrections array, and then the array itself
 * @param char** corrections 	The corrections array to de-allocate
 * @param int count 			The number of corrections in the array
 */
void deallocateCorrections(char** corrections, int count){

	int i = 0;

	/* Frees every word in the corrections, then frees the corrections array */
	for (i = 0; i < count; i++)
		free(corrections[i]);
	
	free(corrections);
}

/** pushErrorNode
 * Push Error type struct onto end of Linked List
 * @param Error* root	Root node for Linked List
 * @param Error* node 	Error node to push to Linked List
 */
void pushErrorNode(Error *root, Error *node) {
	
	/* TODO: Print out error at same time as adding it to list */
	
	while (root != NULL) {
		if (root->next != 0) {
			root = root->next;

			/* Prevent duplicates */
			if (strcmp(root->word, node->word) == 0) {
				return;
			}
		}

		else {
			root->next = node;
			return;
		}
	}
}

/** printErrors
 * Print out list of errors and corrections if applicable
 * @param Error* root 	Root node of Linked List
 */
void printErrors(Error *root) {

	int i = 0;

	/* Runs through the list of detected mispellings until there are no more error structures */
	while (root != NULL) {

		/* Sets the current node to the next one, if it is not already zero */
		if (root->next != 0) {
			root = root->next;

			/* Prints the word that is not found in the dictionary */
			printf("Word not found: %s", root->word);

			/* Prints the corrections found, if any were detected in the dictionary */
			if (strcmp(root->correction[0], "NOCORRECTION") != 0) {
				printf(", possible corrections: ");
				for (i = 0; i < root->correction_no; i++) {
					printf("%s", root->correction[i]);
					if (i != root->correction_no - 1) {
						printf(", ");
					} else {
						printf(".");
					}
				}
			}
			else {
				printf(", no corrections found.");
			}
			printf("\r\n");
		}
		else {
			return;
		}
	}
}

/** getDifference
 * Return how different two words are:
 * 0 : Identical
 * 1 : Potential correction
 * Anything else : Too different
 *
 * @param char* word 	Word to check against
 * @param char* test 	Word to test
 * @return int Representing how different the two words are
 */
int getDifference(char* word, char* test) {
	int temp = 0, i;

	/* If words are identical */
	if (strlen(word) == strlen(test) && strcmp(word, test) == 0) {
		return 0;
	}

	/* If dictionary word is longer than textfile word */
	if (strlen(word) > strlen(test)) return -1;

	/* If textfile word is longer than dictionary word by > 1 */
	if (strlen(test) - strlen(word) > 1) return -1;

	/* If textfile word is one longer than dictionary word and equal up until that point */
	if (strlen(test) - strlen(word) == 1 && strncmp(word, test, strlen(word)) == 0) return 1;

	/* If dictionary word is the same as textfile word */
	if (strlen(word) == strlen(test)) {

		/* If last two letters are swapped but otherwise identical */
		if (strlen(word) >= 2 &&
			word[strlen(word) - 2] == test[strlen(word) - 1] &&
			word[strlen(word) - 1] == test[strlen(word) - 2] &&
			strncmp(word, test, strlen(word) - 2) == 0) {
			return 1;
		}

		/* Compare characters to check single-character differences */
		for (i = 0; i < strlen(word); i++) {
			if (word[i] != test[i]) {
				temp++;
			}
		}
		return temp;
	}

	/* Doesn't match other conditions */
	return -1;
}

/** reformatString
 * Remove punctuation and switch to lowercase
 * @param char* original_string		Word to reformat
 * @param char* processed_string	Where to store formatting word
 */
void reformatString(char *original_string, char *processed_string) {

	int i1, i2;
	char c;

	for (i1 = 0, i2 = 0; i1<strlen(original_string); i1++) {
		c = original_string[i1];

		if ((c >= 'A') && (c <= 'Z'))
			processed_string[i2++] = original_string[i1] - 'A' + 'a';

		if (((c >= 'a') && (c <= 'z')) || (c == '\''))
			processed_string[i2++] = original_string[i1];
	}

	processed_string[i2] = '\0';
}

/** spellCheck
 * Check a word against dictionary, if not found try to find corrections
 * @param char* test 	Word to test against dictionary
 * @param char** dict 	The dictionary to search through
 * @param Error* root 	Root node for Error Linked List
 */
void spellcheck(char* test, char** dict, Error *root) {
	char **corrections, temp[30];
	int i = 0, j = 0, diff, count = 0, flag = 0;
	Error *node;

	/* Ensures no null strings enter the difference checker */
	reformatString(test, temp);
	if (strlen(temp) == 0) return;

	/* Compares the difference of the current word in the text file to every word in the dictionary */
	for (i = 0; i < 1000; i++) {
		diff = getDifference(temp, dict[i]);

		/* Returns if there is no difference between the current word and dictionary word */
		if (diff == 0) {
			return;
		}
	}

	/* Go through the dictionary and finds if the word and dictionary word matches an the error criteria */
	for (i = 0; i < 1000; i++) {
		diff = getDifference(temp, dict[i]);

		/* Finds out how many potential corrections there are for the current word */
		if (diff == 1)
			count += 1;
	}

	/* Allocates memory for the number of corrections to be stored */
	corrections = (char**)malloc(count * sizeof(char*));

	/* Compares the current word with all dictionary words again to copy across the corrections to a 2D array */
	for (i = 0; i < 1000; i++) {
		diff = getDifference(temp, dict[i]);

		if (diff == 1){

			/* Allocates memory for each word and copies the current correction to the 2D array*/
			corrections[j] = (char*)malloc((1 + strlen(dict[i])) * sizeof(char));
			strcpy(corrections[j], dict[i]);
			j++;
		}
	}

	/* If corrections were found, sends corrections to the function creating the new element of the linked list */
	if (count != 0){
		flag = 1;
		node = createErrorNode(temp, corrections, flag, count);
		pushErrorNode(root, node);

		/* Frees the memory used in this function to store corrections to prevent memory leak */
		deallocateCorrections(corrections, count);
		return;
	}

	/* If no corrections are found, new node is created, but the corrections variable is set to a null value in the node */
	flag = 0;
		node = createErrorNode(temp, corrections, flag, count);

	pushErrorNode(root, node);

	/* Frees the memory used in this function to store corrections to prevent memory leak */
	deallocateCorrections(corrections, count);
}

/** readFile
 * Read in all of the lines from a text file and store in array
 * @param char* filename	 	The name of the file to read in from
 * @param char** filecontents	Array to store read lines in
 * @return int	1 if read in fine, 0 if failed
 */
int readFile(char* filename, char** filecontents) {
	int i = 0;
	char temp[200];
	FILE *fileraw;

	/* Assigns the file pointer the memory address of the dictionary file */
	fileraw = fopen("dict.txt", "r");

	/* Exits if the file is null */
	if (fileraw == NULL) return 0;

	while (fscanf(fileraw, "%s", temp) != EOF) {
		filecontents[i] = (char*)malloc((1 + strlen(temp)) * sizeof(char));
		strcpy(filecontents[i], temp);
		i++;
	}
	fclose(fileraw);
	return 1;
}

int main(void) {

	char **dict, c, temp[30];
	FILE *textfile;

	/* Allocates  memory to the 2D array storing the dictionary */
	dict = (char**)malloc(1000 * sizeof(char*));

	/* Create root node, pass dict but ignore it */
	Error *root = createErrorNode("root", dict, 2, 0);

	/* Exits the program with an error if the dictionary file could not be opened */
	if (!readFile("dict.txt", dict)) {
		perror("Failed to open file, exiting...\r\n");
		return -1;
	}

	/* Goes through each word in the text file until the end of the text file is reached and determines if they are erronieous */
	do {
		scanf("%s", temp);
		
		/* TODO: Switch-case for commands, e.g. list all errors */
		
		spellcheck(temp, dict, root);
	} while (strcmp(temp, "exit") != 0);

	/* Frees the dictionary and all root nodes */
	deallocateDict(dict);
	deallocateLinkedList(root);

	return 0;
}
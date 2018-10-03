/* @author: Ankita Sahu */
/* @disclaimer: I am a newbie in git */
/* @input: input.txt*/
/* @output: STD I/O */
/* @limitations: this implementation will work only with positive integers. */
/* @description: Problem statement-

		The task is to write a hashtable program in C using arrays and linked
		lists. Your program must compile and run.

		Input: a file "input.txt" containing a random integer in each line.

		Your program should first create an empty hashtable of 8 buckets
		i.e. an array of 8 elements.  Each bucket is a linked list of numbers
		that belong to that bucket. Your program should then read one number
		at a time from input.txt and insert it into the hashtable (the bucket
		to which a number belongs is its value modulus the number of
		buckets). As soon as the number of entries in any bucket reaches 5, it
		should create a new hashtable with double the number of current
		buckets and insert all the items in the current hashtable into the new
		hashtable and start using the new hashtable.

		After processing all the lines in input.txt, your program should print
		the final hash table and exit. The output should print one line for
		each bucket - the bucket number (starting index 0), followed by a ':',
		followed by the elements in the bucket separated by commas.  For
		example, the first three lines of your output might look something like

*/
/* @observation: with a 8 GB RAM and Ubuntu 18.04, the 10^6 numbers in the range of [0,10^7) were hashable in a hashtable of size 2097152 (# of buckets) efficiently. We can try further for larger and more spread of input numbers */
/* @terms:
		Bucket -> List
		Item -> Number/Node
*/


/* import statements */
#include<stdio.h>
#include <stdlib.h>

int flag = 0 ;	/* variable flag = 0 or 1, to show if any of the lists from hashtable has reached length of 5 */

unsigned int hashtable_size = 8; 	/* initially set to 8 as per problem statement*/

/* Structure of Node in Hashtable */
typedef struct Node{
	unsigned int key;
	struct Node *next; 
}Node;

/* Structure of Hashtable */
typedef struct _Hashtable{
	unsigned int size;
	Node **list;
}Hashtable;

/* function to print Hashtable */
void printHashtable(Hashtable *ht){
	int i =0;
	Node *list=NULL;
	for(i=0; i< ht->size; i++){
		printf("%d:",i);
		for(list = ht->list[i]; list != NULL; list = list->next){
			printf("%d,",list->key);
		}
		printf("\n");
	}
}

/* function to create a new hashtable */
Hashtable* createHashtable(){
	Hashtable *new_ht= malloc(sizeof(Hashtable));
	if (new_ht==NULL){
		printf("!!! Error: Hashtable structure can't be created !!!\n");
		return NULL;
	}
	if((new_ht->list=malloc(sizeof(Node *)*hashtable_size))==NULL){
		printf("!!! Error: Hashtable can't be created !!!\n");		
		return NULL;
	}
	for(int i=0;i<hashtable_size;i++){
		new_ht->list[i] = NULL;
	}
	new_ht->size = hashtable_size;
	// printf("%d\n",new_ht->size);
	// printf("%p\n",(void *)new_ht);
	return new_ht;
}

/* function to calculate the hash */
unsigned int hash(Hashtable* ht, unsigned int key){
	return (key%ht->size); 		//handle negative values in v2.0
}

/* function to search the number in Hashtable.
	If it's present in hashtable, return the list.
	Else
		If length of the list has reached 5, update flag.
		Else do nothing. 

*/
Node* searchKey(Hashtable* ht, int key){
	unsigned int length_of_list = 0; 
	unsigned int hashvalue = hash(ht, key);
	Node *list=NULL;
	for(list = ht->list[hashvalue]; list != NULL; list = list->next){
		if(key == list->key){
			return list;
		}
		length_of_list = length_of_list + 1;
	}
	// printf("length_of_list: %d\n",length_of_list);
	if(length_of_list >= 5)	flag=1;
	return NULL;
}
/* function to copy the hashtables from a small-sized hashtable to larger-sized hashtable */ 
Hashtable* copyHashtable(Hashtable *old_ht,Hashtable *new_ht){
	int insertFlag;
	// printf("new_ht->size : %d\n\n",new_ht->size);
	for(int i=0; i<old_ht->size; i++){
		Node *list = old_ht->list[i];
		while(list){
			Node *new_list;
			// printf("data inside node: %d\n",list->key);
			unsigned int hashvalue = hash(new_ht,list->key);
			if((new_list = malloc(sizeof(Node))) == NULL)	return NULL;
			new_list->key = list->key;
			new_list->next = new_ht->list[hashvalue];
			new_ht->list[hashvalue] = new_list;
			list = list->next;
		}
	}
	new_ht->size = hashtable_size;
	return new_ht;
}

/* function to free up the space occupied by Hashtable */
void freeHashtable(Hashtable *ht){
	Node *list=NULL;
	Node *temp=NULL;
	if(ht==NULL)	return;
	for(int i=0; i<ht->size; i++){
		list = ht->list[i];
		while(list){
			temp = list;
			list = list->next;
			// free(&(temp->key));
			free(temp);
		}
	}
	free(ht->list);
	// free(ht);
}

/* function to insert numbers into Hashtable */
int insertKeyIntoHashtable(Hashtable *ht,int key){
	Node *new_list;
	Node *current_list;
	unsigned int hashvalue = hash(ht,key);
	if((new_list = malloc(sizeof(Node))) == NULL){
		return 1;
	}
	current_list = searchKey(ht,key);
	/* item found */
	if(current_list != NULL) return 2;
	/* item not found, so check with flag whether it has reached length of 5  */
	if(flag){
		hashtable_size = hashtable_size * 2;		
		int insertFlag;
		Hashtable *new_ht = createHashtable();
		flag = 0;
		new_ht = copyHashtable(ht,new_ht);
		if(new_ht){
			freeHashtable(ht);
			*ht = *new_ht;
			// printf("%p, %p\n",(void *) ht,(void *) new_ht);
			// printf("%p, %p\n",(void *) (*ht),(void *) (*new_ht));
			// printHashtable(new_ht);
			insertFlag = insertKeyIntoHashtable(ht,key);
			/* add some o/p handling */
		}else{
			printf("!!! Error: Failure in creating new Hashtable !!!\n");
			return 1;
		}
	}else{
		new_list->key = key;
		new_list->next = ht->list[hashvalue];
		ht->list[hashvalue] = new_list;
	}
	return 0;
}

/* driver function */
int main(){
	Hashtable *ht = createHashtable();
	int insertFlag;
	int i;
	FILE *file = fopen("input.txt","r");
	if(file){
		while (!feof(file)){
			fscanf (file, "%d", &i);
 			insertFlag = insertKeyIntoHashtable(ht,i);
			if(insertFlag==1){
				printf("!!! Error: Can't create memory !!!\n");
				exit(0);
			}
			else if(insertFlag == 2){
				// printf("element is found\n");
				continue;
			}
			else{
				// printf("element is not found. now it's added\n");
				continue;
			}
		}
	}
	printHashtable(ht);
	freeHashtable(ht);
	free(ht);
	fclose(file);
}

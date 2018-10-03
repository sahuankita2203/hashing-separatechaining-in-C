/* @author: Ankita Sahu */
/* @disclaimer: I am a newbie in git */
/* @description: */

#include<stdio.h>
#include <stdlib.h>
int flag = 0 ;
unsigned int hashtable_size = 8;

typedef struct Node{
	unsigned int key;
	struct Node *next; 
}Node;

typedef struct _Hashtable{
	unsigned int size;
	Node **list;
}Hashtable;

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

Hashtable* createHashtable(){
	Hashtable *new_ht= malloc(sizeof(Hashtable));
	if (new_ht==NULL){
		printf("!!! Error: Hashtable structure can't be created !!!\n");
		return NULL;
	}
	// printf("%p\n",hashtable_size);
	// if(malloc(sizeof(Node *)*hashtable_size))	printf("yoooo");
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

unsigned int hash(Hashtable* ht, unsigned int key){
	return (key%ht->size); 		//handle negative values in v2.0
}
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
int main(){
	Hashtable *ht = createHashtable();
	int insertFlag;
	int i;
	FILE *file = fopen("input.txt","r");
	if(file){
		while (!feof(file)){
			fscanf (file, "%d", &i);
			// printf("%d\n",i);
 			insertFlag = insertKeyIntoHashtable(ht,i);
			if(insertFlag==1){
				printf("!!! Error: Can't create memory !!!\n");
				exit(0);
			}
			else if(insertFlag == 2){
				// printf("element found\n");
				continue;
			}
			else{
				// printf("element not found. but added\n");
				continue;
			}
		}
	}
	printHashtable(ht);
	// printf("%d\n", ht->size);
	freeHashtable(ht);
	free(ht);
	fclose(file);
}

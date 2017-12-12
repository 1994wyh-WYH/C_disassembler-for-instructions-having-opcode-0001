/************************************************************************/
/* File Name : lc4_memory.c		 										*/
/* Purpose   : This file implements the linked_list helper functions	*/
/* 			   to manage the LC4 memory									*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"


/*
 * adds a new node to the end of a linked list pointed to by head
 */
int add_to_list (row_of_memory** head,
		 short unsigned int address,
		 short unsigned int contents)
{

	/* allocate memory for a single node */

	/* populate fields in newly allocated node w/ address&contents */

	/* if head==NULL, node created is the new head of the list! */

	/* otherwise, traverse linked list until we reach the end */

	/* add node to the end of list */

	/* return 0 for success, -1 if malloc fails */

	row_of_memory* newNode=malloc(sizeof(row_of_memory));
	if(newNode==NULL){
		return -1;
	}
	newNode->address=address;
	newNode->contents=contents;
	newNode->label=NULL;
	newNode->assembly=NULL;
	newNode->next=NULL;

	if(*head==NULL){
		*head=newNode;
	}
	else{
		//printf("adding to list\n");
		row_of_memory* curr=*head;
		while(curr->next!=NULL){
			if(curr->next->next!=NULL && curr->next->address<address)
				curr=curr->next;
			else
				break;
		}

		if(curr->address<=address){
			if(curr->next==NULL){
				//printf("curr next is null\n");
				curr->next=newNode;
			}
			else{
				if(curr->next->address>address){
				//printf("curr next is not null%d\n", curr->next==NULL);
				row_of_memory* currn=curr->next;
				curr->next=newNode;
				newNode->next=currn;
				//printf("done swaping\n");
				}
				else{
					row_of_memory* currn=curr->next;
					currn->next=newNode;
				}
			}
		}
		else{
			//printf("adding to head\n");
			newNode->next=curr;
			*head=newNode;
		}
	}
	//printf("returning from adding\n");
	return 0 ;
}


/*
 * search linked list by address field, returns node if found
 */
row_of_memory* search_address (row_of_memory* head,
			       short unsigned int address )
{
	/* traverse linked list, searching each node for "address"  */

	/* return pointer to node in the list if item is found */

	/* return NULL if list is empty or if "address" isn't found */
	row_of_memory* curr=head;
	//printf("searching list\n");
	//printf("%d\n",curr==NULL);
	while(curr!=NULL){
		//printf("searching list:traversing\n");
		if(curr->address!=address){
			curr=curr->next;
		}
		else{
			break;
		}
	}
	//printf("searching list ends\n");
	if(curr==NULL){
		//printf("returning null\n");
		return NULL;
	}
	return curr;
}

/*
 * search linked list by opcode field, returns node if found
 */
row_of_memory* search_opcode  (row_of_memory* head,
				      short unsigned int opcode  )
{
	/* traverse linked list until node is found with matching opcode
	   AND "assembly" field of node is empty */

	/* return pointer to node in the list if item is found */

	/* return NULL if list is empty or if no matching nodes */
	row_of_memory* curr=head;
	//printf("searching opcode\n");
	//printf("%d\n", curr==NULL);
	while(curr!=NULL){
		//printf("1seaching opcode:traversing\n");
		//printf("contents %x\n", curr->contents);
		if((((curr->contents)/0x1000)!=opcode) || (curr->assembly!=NULL)){
			//printf("2seaching opcode:traversing\n");
			curr=curr->next;
		}
		else{
			break;
		}
	}
	if(curr==NULL){
		return NULL;
	}
	//printf("2seaching opcode:traversing\n");
	return curr;
	
}


void print_list (row_of_memory* head )
{
	/* make sure head isn't NULL */

	/* print out a header */

	/* traverse linked list, print contents of each node */
	row_of_memory* curr=head;
	printf("<label>\t\t<address>\t<contents>\t<assembly>\n");
	while(curr!=NULL){
		printf("%s\t\t%x\t\t%x\t\t%s\n", curr->label, curr->address, curr->contents, curr->assembly);
		curr=curr->next;
	}
	return ;
}

/*
 * delete entire linked list
 */
int delete_list    (row_of_memory** head )
{
	/* delete entire list node by node */
	/* if no errors, set head = NULL upon deletion */

	/* return 0 if no error, -1 for any errors that may arise */
	row_of_memory* curr=*head;
	//printf("deleting list\n");
	while(curr!=NULL){
		row_of_memory* temp=curr;
		curr=curr->next;
		free(temp->label);
		free(temp->assembly);
		free(temp);
	}
	if(curr==NULL){
		*head=NULL;
	}
	else{
		return -1;
	}
	return 0 ;
}
